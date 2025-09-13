/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:50:47 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/13 19:02:31 by jegerman         ###   ########.fr       */
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
# define DEFAULT_PATH "/bin:/usr/bin"
# define ERR_SYNTAX "minishell: syntax error near unexpected token `%s'\n"

extern uint32_t	g_exit_status; // Why a straight g_?? Real G or just a bitch-ass n*?

typedef struct sigaction	t_sigaction;

typedef enum e_max
{
	TOK_MAX = 1024,
	CMD_MAX = 128,
	RED_MAX = 32,
	ID_LMAX = 128,
}	t_max;

typedef enum e_tokt
{
	TOK_EOL, TOK_WORD, TOK_PIPE,
	TOK_IRED, TOK_IRED_HD,
	TOK_ORED, TOK_ORED_AP
}	t_tokt;

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
	int		fds[2];
}	t_red;

typedef struct s_cmd
{
	char	**argv;
	t_red	ireds[RED_MAX];
	int		ilen;
	t_red	oreds[RED_MAX];
	int		olen;
	pid_t	pid;
	int		ifds[2];
	int		ofds[2];
}	t_cmd;

typedef struct s_core
{
	t_cmd		cmds[CMD_MAX];
	int			cmd_nbr; // ???
	uint16_t	*exit; // ???
}	t_core;

int		lex_is_quote(int c);
int		lex_is_op(int c);
int		lex_is_sep(int c);

int		lex_print_tokens(t_tok *tokens); // REMOVE
int		lex_tokenize_line(char *line, t_tok *tokens);

int		psr_is_ired(t_tok *token);
int		psr_is_ored(t_tok *token);
int		psr_add_reds(t_tok *token, t_cmd *cmd);

int		psr_add_cmd(t_tok *token, t_cmd *cmd);

int		psr_error_check(t_tok *tokens);

char	*psr_create_word(t_tok *token);

int		psr_parse_line(char *line, t_core *core);

int		sig_init_handlers(void);

int		bi_exit(char *line);

int		ui_loop_prompt(t_core *core);

#endif
