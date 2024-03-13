/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:09:52 by ggay              #+#    #+#             */
/*   Updated: 2024/03/13 19:39:21 by jgermany         ###   ########.fr       */
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

# define DEFAULT_PATH "/bin:/usr/bin"
# define INTF_PROMPT "prompt> "
# define SPECIAL_CHARS "<>|"

typedef struct sigaction	t_sigaction;

typedef struct s_cmd
{
	char	*cmd;
	char	**envp;
	pid_t	pid;
	int		in_fds[2];
	int		out_fds[2];
}	t_cmd;

typedef struct s_core
{
	t_cmd		*cmds;
	uint16_t	*exit;
}	t_core;

typedef enum e_chartype
{
	CHR_SPACE,
	CHR_SPECIAL,
	CHR_RDIN,
	CHR_RDOUT,
	CHR_HEREDOC,
	CHR_APPEND,
	CHR_SQUOTE,
	CHR_DQUOTE
}	t_chartype;

typedef enum e_endtype
{
	END_SQUOTE,
	END_DQUOTE,
	END_LINE,
	END_SPECIAL
}	t_endtype;

extern uint16_t	g_exit_status;

int	sigmgr_init_handlers(void);

int	bltn_exit(char *line);

int	psr_is_char(t_chartype type, char *line);

int	psr_extract_cmd(int *i, char *line);
int	psr_extract_path(char *name, int *i, char *line);
int	psr_parse_line(char *line);

int	intf_loop_prompt(t_core *core);

#endif
