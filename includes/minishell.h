/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:50:47 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/28 21:39:43 by jegerman         ###   ########.fr       */
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

# define UI_PROMPT "minishell> "

# define ENV_DFLT_PATH "/bin:/usr/bin" // DEFAULT_PATH

# define ERR_SYNTAX "minishell: syntax error near unexpected token `%s'\n"
# define ERR_GNR "minishell: %s\n"
# define ERR_PTH "minishell: %s: %s\n"
# define ERR_CMD "minishell: %s: command not found\n"

# define FL_PRMS 00664
# define FL_IRED O_RDONLY
# define FL_ORED (O_CREAT | O_TRUNC | O_WRONLY)
# define FL_ORED_AP (O_CREAT | O_APPEND | O_WRONLY)


extern uint32_t	g_exit_status; // Why a straight g_?? Real G or just a bitch-ass n*?

typedef struct sigaction	t_sigaction;

typedef enum e_max
{
	TOK_MAX = 2048,
	CMD_MAX = 256,
	RED_MAX = 64,
	ID_LMAX = 1024,
	PFD_MAX = 2,
	EXW_MAX = 16,
}	t_max;

typedef enum e_tokt
{
	TOK_EOL, TOK_WORD, TOK_PIPE,
	TOK_IRED, TOK_IRED_HD,
	TOK_ORED, TOK_ORED_AP
}	t_tokt;

typedef enum e_cflg
{
	FLG_CMDS = (1 << 0),
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
	// int		fds[PFD_MAX];
}	t_red;

typedef struct s_cmd
{
	char	**argv;
	bool	xready;
	pid_t	pid;
	t_red	ireds[RED_MAX];
	t_red	oreds[RED_MAX];
	int		ifds[PFD_MAX];
	int		ofds[PFD_MAX];
}	t_cmd;

typedef struct s_core
{
	t_cmd		cmds[CMD_MAX];
	int			cmd_pmax;
	uint16_t	*exit; // ???
	uint32_t	flags;
}	t_core;

int		lex_is_quote(int c);
int		lex_is_op(int c);
int		lex_is_sep(int c);

int		lex_tokenize_line(char *line, t_tok *toks);

int		psr_cleanup_red(t_red *red, int *fds);
int		psr_cleanup_cmds(t_core *core);

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
int		fmgr_close(int pos, int *fds);

int		fmgr_set_hdocs(int *ifds, t_red *red);
int		fmgr_set_reds(t_core *core);

int		utl_cleanup(t_cflg flags, t_core *core);
char	*utl_shitoa(unsigned int nbr, char *store);

int		sig_init_handlers(void);

int		bi_exit(char *line);

int		ui_loop_prompt(t_core *core);

#endif
