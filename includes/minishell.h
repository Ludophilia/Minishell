/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:09:52 by ggay              #+#    #+#             */
/*   Updated: 2023/12/18 13:06:00 by jgermany         ###   ########.fr       */
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

# define INTERF_PROMPT "minishell > "

typedef struct sigaction t_sigaction;

typedef struct s_cmd
{
	char	*path;
	char	**envp;
	pid_t	pid;
	int		infd[2];
	int		outfd[2];
}	t_cmd;

typedef struct s_core
{
	char	*line;
	t_cmd	*cmds;
}	t_core;

#endif
