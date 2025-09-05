/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:09:52 by ggay              #+#    #+#             */
/*   Updated: 2025/09/05 16:44:24 by jegerman         ###   ########.fr       */
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

extern uint32_t	g_exit_status;

typedef struct sigaction	t_sigaction;

typedef enum e_tokt
{
	TOK_EOL, TOK_WORD,
	TOK_IRED, TOK_IRED_HD,
	TOK_ORED, TOK_ORED_AP,
	TOK_PIPE,
}	t_tokt;

typedef struct s_tok
{
	enum e_tokt		type;
	char			*start;
	int				len;
}	t_tok;

typedef struct s_cmd
{
	char	*cmd;
	char	**envp;
	pid_t	pid;
	int		in_fds[2];
	int		out_fds[2];
}	t_cmd;

// 5/9 - Isn't that a little... too small? 
typedef struct s_core
{
	t_cmd		*cmds; // Arrays? Lists?
	// uint16_t	*exit; // ???
}	t_core;

int	lex_is_quote(int c);
int	lex_is_op(int c);
int	lex_is_sep(int c);
int	lex_tokenize_line(char *line);

int	sig_init_handlers(void);

int	bi_exit(char *line);

int	ui_loop_prompt(t_core *core);

#endif
