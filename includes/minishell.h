/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:50:47 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/13 23:02:31 by jegerman         ###   ########.fr       */
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
# include <limits.h>
# include <sys/stat.h>
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

# define ERR_XNUM "exit: %s: numeric argument required\n"
# define ERR_XMNY "exit: too many arguments\n"
// # define ERR_UINV "unset: `%s': not a valid identifier\n"

extern uint32_t	g_sig;

typedef enum e_flo
{
	FL_PRMS = 00664,
	FL_IRED = O_RDONLY,
	FL_ORED = (O_CREAT | O_TRUNC | O_WRONLY),
	FL_ORED_AP = (O_CREAT | O_APPEND | O_WRONLY)
}	t_flo;

typedef enum e_max
{
	TOK_MAX = 2048,
	CMD_MAX = 128,
	RED_MAX = 128,
	ID_LMAX = 256,
	EXW_MAX = 16,
	PATH_MAX = 4096
}	t_max;

typedef enum e_tokt
{
	TOK_EOL = 0,
	TOK_WORD,
	TOK_PIPE,
	TOK_IRED,
	TOK_IRED_HD,
	TOK_ORED,
	TOK_ORED_AP
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

typedef struct s_cmd
{
	t_red			reds[RED_MAX];
	bool			xready;
	pid_t			pid;
	// int		is_bltn; // ???
	char			**argv;
	int				argc;
	int				ifd;
	int				ofd;
}	t_cmd;

typedef struct s_core
{
	t_cmd			cmds[CMD_MAX];
	int				cmd_pmax;
	int				cmd_xrdy;
	uint32_t		flags;
	uint8_t			exit;
	t_env			*env;
}	t_core;

int		lex_is_quote(int c);
int		lex_is_op(int c);
int		lex_is_sep(int c);
int		lex_tokenize_line(char *line, t_tok *toks);

int		psr_is_ired(t_tok *tok);
int		psr_is_ored(t_tok *tok);
int		psr_is_outq(int c, int *q);
int		psr_is_envv(char *c, int ct, int q);
int		psr_is_envv_chr(int c, int pos);
int		psr_envv_value_len(char *start, int *j);
int		psr_copy_envv_value(char *start, char *word, int *j);
int		psr_cleanup_cmds(t_cflg flags, t_core *core);
char	*psr_create_word(t_tok *tok, t_tokt context);
int		psr_add_reds(t_tok *tok, t_cmd *cmd);
int		psr_add_cmd(t_tok *tok, t_cmd *cmd);
int		psr_error_check(t_tok *toks);
int		psr_parse_line(char *line, t_core *core);

int		fmgr_access(char *path, int type);
int		fmgr_open(char *path, int openflags, mode_t openmode);
int		fmgr_pipe(int fds[2]);
int		fmgr_close(int *xfd);
int		fmgr_dup2(int old_fd, int new_fd);
int		fmgr_set_hdocs(int *ifds, t_red *red);
int		fmgr_set_pipe(int pos, int pmax, t_cmd *cmd);
int		fmgr_set_red(int *xfd, int openflags, t_red *red);
int		fmgr_set_reds(t_core *core);

int		utl_free_strs(int from_id, char **strs);
int		utl_cleanup(t_cflg flags, t_core *core);
char	*utl_shitoa(unsigned int nbr, char *store);
int		utl_free(void *ptr);

int		sig_init_prompt(void);
int		sig_init_child(void);

t_env	*env_new(char *key, char *value);
void	env_add(t_env **list, t_env *new);
t_env	*env_dup(char **envp);
void	env_add_node(t_env **env, const char *key, const char *value);
char	*env_get(t_env *env, const char *key);
void	env_set(t_env **env, const char *key, const char *value);
int		env_is_identifier(const char *str);
t_env	*env_dup_fail(char *key, char *val, t_env *list);
void	env_free(t_env *env);

int		bi_cd(t_core *core, t_cmd *cmd);
int		bi_echo(t_cmd *cmd, int fd);
int		bi_env(t_env *env, int fd);
int		bi_exit(t_core *core, t_cmd *cmd);
int		bi_export(t_cmd *cmd, t_env **env);
int		bi_pwd(int fd);
int		bi_unset(t_cmd *cmd, t_env **env);

int		exc_check_path(char **argv, char **envp);
int		exc_exec_cmds(t_core *core);
int		exc_exec_builtin(t_core *core, t_cmd *cmd, int fd);
int		exc_is_builtin(char *arg);

int		loop_prompt(t_core *core);

#endif
