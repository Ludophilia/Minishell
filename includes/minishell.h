/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:50:47 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/09 20:21:10 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "libft.h"

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <stdint.h>
# include <string.h>

# define UI_PROMPT "\033[1;35mMinishell> \033[0m"

# define ENV_DFLT_PATH "/bin:/usr/bin"

# define ERR_USAGE "usage: ./minishell\n"

# define ERR_SYNTAX "syntax error near unexpected token `%s'\n"
# define ERR_GNR "%s\n"
# define ERR_PTH "%s: %s\n"
# define ERR_CMD "%s: command not found\n"
# define ERR_ECMD "'%c': command not found\n"

# define ERR_BNUM "%s: %s: numeric argument required\n"
# define ERR_BMNY "%s: too many arguments\n"
# define ERR_BINV "%s: `%s': not a valid identifier\n"

extern volatile sig_atomic_t	g_sig;

typedef enum e_flo
{
	FL_PRMS = 00664,
	FL_IRED = O_RDONLY,
	FL_ORED = (O_CREAT | O_TRUNC | O_WRONLY),
	FL_ORED_AP = (O_CREAT | O_APPEND | O_WRONLY)
}	t_flo;

typedef enum e_exit
{
	EX_S = EXIT_SUCCESS,
	EX_F = EXIT_FAILURE,
	EX_CNEX = 126,
	EX_CNFD = 127,
}	t_exit;

typedef enum e_max
{
	TOK_MAX = 1024,
	CMD_MAX = 128,
	RED_MAX = 64,
	ID_LMAX = 256,
	EXW_MAX = 16,
	PTH_MAX = 4096,
	NOD_MAX = 64,
}	t_max;

typedef enum e_tokt
{
	TOK_EOL = 0,
	TOK_WORD = 1,
	TOK_PIPE = 2,
	TOK_IRED = 3,
	TOK_IRED_HD = 4,
	TOK_ORED = 5,
	TOK_ORED_AP = 6,
	TOK_AND = 7,
	TOK_OR = 8,
	TOK_SUBO = 9,
	TOK_SUBC = 10
}	t_tokt;

typedef enum e_cflg
{
	FLG_NONE = 0,
	FLG_CORE = (1 << 0),
	FLG_ENV = (1 << 1),
	FLG_AST = (1 << 2),
	FLG_REDS = (1 << 3),
}	t_cflg;

typedef enum e_astt
{
	AST_NONE = 0,
	AST_AO = 1,
	AST_PI,
	AST_SUB,
	AST_CMD
}	t_astt;

typedef struct s_count
{
	int					f;
	int					i;
	int					j;
	int					k;
}	t_cnt;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}	t_env;

typedef struct s_tok
{
	enum e_tokt			type;
	char				*start;
	int					len;
}	t_tok;

typedef struct s_red
{
	t_tokt				type;
	t_tok				*word_tk; // t_tok or t_tok *
	char				*word;
}	t_red;

typedef struct s_cmd
{
	t_red				reds[RED_MAX];
	int					red_pmax;
	// bool				xready;
	t_tok				**argv_tk; // t_tok * or t_tok **
	char				**argv;
	int					argc;
	int					ifd;
	int					ofd;
}	t_cmd;

typedef struct s_astn
{
	t_astt				type;
	t_tokt				op;
	void				*content;
	void				*left;
	void				*right;
}	t_astn;

typedef struct s_core
{
	t_astn				*ast;
	t_astn				*stash[NOD_MAX];
	t_tok				toks[TOK_MAX];
	int					cmds;
	uint32_t			flags;
	uint8_t				exit;
	t_env				*env;
	char				**envp;
}	t_core;

int		init_core(t_core *core, char **envp);
int		init_isatty(void);

int		utl_free_strs(int from_id, char **strs);
int		utl_cleanup(t_cflg base_flags, t_cflg excl_flags, t_core *core);
char	*utl_itoa(unsigned int nbr, char *store);
int		utl_free(void *ptr);
int		utl_exit(int status, t_core *core);

int		lex_is_quote(char *c);
int		lex_is_op(char *c);
int		lex_is_sep(char *c);
int		lex_tokenize_line(char *line, t_tok *toks);

int		lex_istok(t_tokt type, t_tok *tok);
int		lex_isred(t_tok *tok);
int		lex_issub(t_tok *tok);
int		lex_isop(t_tok *tok);

int		lex_synterr(t_tokt type, t_tok *tok);
int		lex_subtok_check(int *pos, int *opn, t_tok *tok, t_tok *next);
int		lex_error_check(t_tok *toks, t_core *core);

int		exp_is_outq(int c, int *q);
int		exp_is_envv(char *c, int token, int q);
int		exp_is_envv_chr(int c, int pos);
int		exp_envv_value_len(char *start, int *j, t_core *core);
int		exp_copy_envv_value(char *start, char *word, int *j, t_core *core);
char	*exp_create_word(t_tok *tok, t_tokt context, t_core *core);

int		psr_cleanup_ast(t_astn *root);
t_astn	*psr_new_astn(t_astt type);
int		psr_build_ast(t_tok *toks, t_core *core);

int		psr_rdp_scmd(t_cnt *c, t_tok *toks, t_core *core, t_astn *node);
t_astn	*psr_rdp_line(t_cnt *c, t_tok *toks, t_core *core);

int		psr_parse_line(char *line, t_core *core);

int		fmgr_open(char *path, int openflags, mode_t openmode);
int		fmgr_pipe(int fds[2]);
int		fmgr_close(int *xfd);
int		fmgr_dup2(int old_fd, int new_fd);
int		fmgr_set_xfds(t_cmd *cmd, t_core *core);

int		sig_init_prompt(void);
int		sig_init_child(void);
int		sig_init_exec(void);

int		bi_cd(t_core *core, t_cmd *cmd);
int		bi_echo(t_cmd *cmd, int fd);
int		bi_env(t_env *env, int fd);
int		bi_exit(t_core *core, t_cmd *cmd);
int		bi_export(t_cmd *cmd, t_env **env);
int		bi_pwd(int fd);
int		bi_unset(t_cmd *cmd, t_env **env);

int		exc_wait_pid(pid_t	pid, t_core *core);

int		exc_err_ecmd(char c, t_core *core);
int		exc_err_cmd(char *path, t_core *core);
int		exc_err_path(int access_test, char *path, t_core *core);
int		exc_err_pathg(char *path, char *strerr, t_core *core);
char	*exc_build_abspath(char **strs);
int		exc_load_envpath(char **envp, char ***paths);
int		exc_check_path(char **argv, char **envp, t_core *core);
int		exc_close_pipes(int ifd, int ofd, t_core *core);
int		exc_process_reds(int *ifd, int *ofd, t_astn *root, t_core *core);
int		exc_exec_scmd(int ifd, int ofd, t_astn *root, t_core *core);
int		exc_if_builtin(t_cmd *cmd, t_core *core);
int		exc_exec_ast(int ifd, int ofd, t_astn *root, t_core *core);

int		loop_prompt(t_core *core);

t_env	*env_new_node(const char *key, const char *value);
int		env_add_start(t_env **env_list, const char *key, const char *value);
int		env_add_end(t_env **env_list, const char *key, const char *value);
t_env	*env_dup(char **envp);
int		env_del_node(t_env *node, t_env *prev, t_env **env_list);
int		env_free_node(t_env *node);
int		env_free_all(t_env *node);
int		env_is_identifier(const char *str);
char	*env_get(t_env *env_list, const char *key);
int		env_set(t_env **env_list, const char *key, const char *value);
char	**env_get_envp(t_env *env_list, t_core *core);
int		env_cleanup(t_core *core);

#endif
