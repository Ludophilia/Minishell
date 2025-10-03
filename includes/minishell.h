/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:50:47 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/03 17:26:27 by jegerman         ###   ########.fr       */
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
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <stdint.h>

# define UI_MAGENTA "\033[1;35m"
# define UI_RESET "\033[0m"
# define UI_PROMPT "\033[1;35mMinishell> \033[0m"

# define ENV_DFLT_PATH "/bin:/usr/bin" // DEFAULT_PATH

# define ERR_SYNTAX "syntax error near unexpected token `%s'\n"
# define ERR_GNR "%s\n"
# define ERR_PTH "%s: %s\n"
# define ERR_CMD "%s: command not found\n"
# define ERR_ECMD "'%c': command not found\n"

# define FL_PRMS 00664
# define FL_IRED O_RDONLY
# define FL_ORED (O_CREAT | O_TRUNC | O_WRONLY)
# define FL_ORED_AP (O_CREAT | O_APPEND | O_WRONLY)

extern uint32_t	g_exit_status; // signals, please improve this...

typedef struct sigaction	t_sigaction;

typedef enum e_max
{
	TOK_MAX = 2048,
	CMD_MAX = 128,
	RED_MAX = 128,
	ID_LMAX = 256,
	EXW_MAX = 16,
}	t_max;

typedef enum e_tokt
{
	TOK_EOL, TOK_WORD, TOK_PIPE,
	TOK_IRED, TOK_IRED_HD,
	TOK_ORED, TOK_ORED_AP
}	t_tokt;

// typedef enum e_bid
// {
// 	BID_
// }	t_bid;

typedef enum e_cflg
{
	FLG_CMDS = (1 << 0),
	FLG_REDS = (1 << 1),
}	t_cflg;

typedef struct s_tok
{
	enum e_tokt	type;
	char		*start;
	int			len;
}	t_tok;

typedef struct s_red
{
	t_tokt	type;
	char	*word;
}	t_red;

typedef struct s_cmd
{
	t_red	reds[RED_MAX];
	bool	xready;
	// pid_t	pid;
	// int		is_bltn;
	char	**argv;
	int		ifd;
	int		ofd;
}	t_cmd;

typedef struct s_core
{
	t_cmd		cmds[CMD_MAX];
	int			cmd_pmax;
	char		**envp; // provisory
	uint8_t		exitv; // later in the execution pipeline
	uint32_t	flags;
}	t_core;

int		lex_is_quote(int c);
int		lex_is_op(int c);
int		lex_is_sep(int c);

int		lex_tokenize_line(char *line, t_tok *toks);

int		psr_cleanup_cmds(t_cflg flags, t_core *core);

int		psr_is_ired(t_tok *tok);
int		psr_is_ored(t_tok *tok);
int		psr_is_outq(int c, int *q);
int		psr_is_envv(char *c, int ct, int q);
int		psr_is_envv_chr(int c, int pos);

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

int		utl_cleanup(t_cflg flags, t_core *core);
char	*utl_shitoa(unsigned int nbr, char *store);

int		exc_check_path(char **argv, char **envp);
int		exc_exec_cmds(t_core *core);

int		utl_print_cmds(t_core *core); // REMOVE

int		sig_init_handlers(void);

int		bi_exit(char *line);

int		ui_loop_prompt(t_core *core);

#endif
