/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:50:47 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/12 16:56:45 by jegerman         ###   ########.fr       */
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
	EX_SUCC = 0,
	EX_FAIL = 1,
	EX_CNFD = 127,
}	t_exit;

typedef enum e_max
{
	TOK_MAX = 2048,
	CMD_MAX = 128,
	RED_MAX = 64,
	ID_LMAX = 256,
	EXW_MAX = 16,
	PTH_MAX = 4096
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
	FLG_CMDS = (1 << 0),
	FLG_REDS = (1 << 1),
	FLG_CORE = (1 << 2),
	FLG_ENV = (1 << 3),
	FLG_ALL = (FLG_CMDS | FLG_REDS | FLG_CORE),
}	t_cflg;

typedef struct s_tok
{
	enum e_tokt		type;
	char			*start;
	int				len;
}	t_tok;

typedef struct s_count
{
	int		f;
	int		i;
	int		j;
	int		k;
}	t_cnt;

typedef struct s_red
{
	t_tokt			type;
	char			*word;
}	t_red;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_astt
{
	AST_AO = 1,
	AST_PI,
	AST_SUB,
	AST_CMD
}	t_astt;

typedef struct s_astn
{
	t_astt				type;
	t_tokt				op;
	void				*left; // 12/12 - void *?
	void				*right;
}	t_astn;

typedef struct s_cmd
{
	t_red			reds[RED_MAX];
	int				red_pmax;
	bool			xready; // 12/12 - Useless?
	pid_t			pid;
	char			**argv;
	int				argc;
	int				ifd;
	int				ofd;
}	t_cmd;


// typedef struct s_cmdn
// {
// 	struct s_logn	*sub;
// 	t_cmd			*cmd;
// }	t_cmdn;

// typedef struct s_pipn
// {
// 	t_tokt			op;	
// 	t_cmdn			*left;
// 	t_cmdn			*right;
// }	t_pipn;

// typedef struct s_logn
// {
// 	t_tokt			op;
// 	t_pipn			*left;
// 	t_pipn			*right;
// }	t_logn;

typedef struct s_core
{
	t_astn			*ast;
	int				cmd_pmax; // 9/12 - Broken
	
	int				cmd_xrdy; // Nbrs of xready cmds?

	uint32_t		flags;
	uint8_t			exit;
	t_env			*env;
	char			**envp;
}	t_core;

int		init_core(t_core *core, char **envp);
int		init_cleanup_core(t_core *core);
int		init_isatty(void);

int		lex_is_quote(char *c);
int		lex_is_op(char *c);
int		lex_is_sep(char *c);
int		lex_tokenize_line(char *line, t_tok *toks);

int		psr_istok(t_tokt type, t_tok *tok);
int		psr_isred(t_tok *tok);
int		psr_isop(t_tok *tok);
int		psr_synterr(t_tokt type, t_tok *tok);
int		psr_error_check(t_tok *toks, t_core *core);

int		psr_is_outq(int c, int *q);
int		psr_is_envv(char *c, int ct, int q);
int		psr_is_envv_chr(int c, int pos);

int		psr_envv_value_len(char *start, int *j, t_core *core);
int		psr_copy_envv_value(char *start, char *word, int *j, t_core *core);
char	*psr_create_word(t_tok *tok, t_tokt context, t_core *core);
int		psr_fill_cmd(t_cnt *c, t_tok *toks, t_core *core, t_cmd *cmd);
int		psr_cleanup_cmds(t_cflg flags, t_core *core);
int		psr_parse_line(char *line, t_core *core);

int		fmgr_access(char *path, int type);
int		fmgr_open(char *path, int openflags, mode_t openmode);
int		fmgr_pipe(int fds[2]);
int		fmgr_close(int *xfd);
int		fmgr_dup2(int old_fd, int new_fd);
int		fmgr_set_hdocs(int *ifds, t_red *red, t_core *core);
int		fmgr_set_pipe(int pos, int pmax, t_cmd *cmd);
int		fmgr_set_red(int *xfd, int openflags, t_red *red, t_core *core);
int		fmgr_set_reds(t_core *core);

int		utl_free_strs(int from_id, char **strs);
int		utl_cleanup(t_cflg flags, t_core *core);
char	*utl_itoa(unsigned int nbr, char *store);
int		utl_free(void *ptr);
int		utl_exit(int status, t_core *core);

int		sig_init_prompt(void);
int		sig_init_child(void);
int		sig_init_exec(void);

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

int		bi_cd(t_core *core, t_cmd *cmd);
int		bi_echo(t_cmd *cmd, int fd);
int		bi_env(t_env *env, int fd);
int		bi_exit(t_core *core, t_cmd *cmd);
int		bi_export(t_cmd *cmd, t_env **env);
int		bi_pwd(int fd);
int		bi_unset(t_cmd *cmd, t_env **env);

int		exc_check_path(char **argv, char **envp);
int		exc_wait_cmds(t_core *core);
int		exc_exec_cmds(t_core *core);
int		exc_if_builtin(t_cmd *cmd, t_core *core);

int		loop_prompt(t_core *core);

#endif
