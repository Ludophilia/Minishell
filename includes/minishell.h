/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:09:52 by ggay              #+#    #+#             */
/*   Updated: 2023/12/27 18:08:15 by jgermany         ###   ########.fr       */
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

# define DEFAULT_PATH "/bin:/usr/bin"
# define INTF_PROMPT "minishell > "
# define SPECIAL_CHARS "<>()|\"'&"

typedef struct sigaction t_sigaction;

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
	t_cmd	*cmds;
}	t_core;

enum e_typ
{
	TY_SPAC,
	TY_SPEC,
	TY_RDIN,
	TY_RDHRD,
	TY_RDAPN,
	TY_RDOUT,
	TY_CMDEND
};

int	sig_init_handlers(void);

int	blt_exit(char *line);

int	psr_is_type(enum e_typ type, char *line);
int	psr_extract_cmd(int *i, char *line);
int	psr_extract_path(char *name, int *i, char *line);
int	psr_parse_line(char *line);

int	intf_loop_prompt(void);

#endif
