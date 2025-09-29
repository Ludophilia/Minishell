/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/29 21:57:34 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	pgmb_free_strs(int from_id, char **strs)
// {
// 	int	i;

// 	if (strs == NULL)
// 		return (-1);
// 	i = 0;
// 	while (strs[from_id + i])
// 		free(strs[from_id + i++]);
// 	free(strs);
// 	return (1);
// }

static int	pgmb_wait_cmds(int i, t_pgb *prgs)
{
	int	wait_stat;
	int	chld_fails;

	chld_fails = 0;
	while (--i >= 0)
	{
		if (waitpid(prgs[i].pid, &wait_stat, 0) == -1
			|| ((wait_stat >> 8) & 0xFF) == EXIT_FAILURE)
			++chld_fails;
	}
	if (chld_fails != 0)
		return (-1);
	return (0);
}

static int	pgmb_exec_cmd(t_pgb *prgs, int i, char **envp)
{
	char	**cmd_args;

	cmd_args = ft_split(prgs[i].cmd, ' ');
	if (cmd_args == NULL
		|| (*cmd_args == NULL && ft_eprintf(ERR_CMD, NULL))
		|| ptbb_check_path(cmd_args, envp) == -1
		|| dup2(prgs[i].in[0], 0) == -1
		|| dup2(prgs[i].out[1], 1) == -1)
	{
		pgmb_free_strs(0, cmd_args);
		fmgb_closeall(0, DIR_FWD, prgs);
		return (-1);
	}
	if (fmgb_closeall(0, DIR_FWD, prgs) == -1
		|| execve(*cmd_args, cmd_args, envp) == -1)
	{
		pgmb_free_strs(0, cmd_args);
		return (-1);
	}
	return (0);
}

int	exc_exec_cmds(t_core *core)
{
	pid_t	pid;
	int		i;

	i = -1;
	while (prgs[++i].cmd)
	{
		pid = fork();
		if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		{
			pgmb_wait_cmds(i, prgs);
			return (-1);
		}
		if (pid == 0 && pgmb_exec_cmd(prgs, i, envp) == -1)
			exit(EXIT_FAILURE);
		prgs[i].pid = pid;
	}
	if (pgmb_wait_cmds(i, prgs) == -1)
		return (-1); // close fds
	return (0);
}