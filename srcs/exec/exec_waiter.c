/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_waiter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:55:22 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/11 17:05:48 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exc_wait_pid(pid_t	pid, t_core *core)
{
	int	wstat;
	int	sig;

	if (waitpid(pid, &wstat, 0) == -1)
		return (-1);
	sig = 0;
	if (WIFEXITED(wstat))
		core->exit = WEXITSTATUS(wstat);
	else if (WIFSIGNALED(wstat))
	{
		sig = WTERMSIG(wstat);
		if (sig == SIGQUIT)
			write(1, "\n", 1);
		if (sig == SIGPIPE)
			core->exit = 1;
		else
			core->exit = 128 + sig;
	}
	return (0);
}
