/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_waiter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:55:22 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/04 00:48:03 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exc_process_exit(int wstat, t_core *core)
{
	int		sig;

	sig = 0;
	if (WIFEXITED(wstat))
		core->exit = WEXITSTATUS(wstat);
	else if (WIFSIGNALED(wstat))
	{
		sig = WTERMSIG(wstat);
		if (sig == SIGQUIT)
			write(STDOUT_FILENO, "\n", 1);
		if (sig == 13)
			core->exit = 1;
		else
			core->exit = 128 + sig;
	}
}

int	exc_wait_cmds(pid_t	pid, t_core *core)
{
	// t_cmd	*cmd;
	int		wstat;
	// int		i;

	// utl_cleanup(FLG_REDS, core);
	// i = -1;
	// while (++i < (core->cmds + 1))
	// {
	// cmd = core->cmds + i;
	// if (cmd->xready == false)
	// 	continue ;
	if (waitpid(pid, &wstat, 0) == -1)
		return (-1);
	exc_process_exit(wstat, core); // 4/01: Nezlocked.
	// }
	// (void)core;
	return (0);
}
