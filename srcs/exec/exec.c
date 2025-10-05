/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/06 00:52:50 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exc_wait_cmds(int i)
{
	int	wstat;
	int	cfails;

	cfails = 0;
	while (--i >= 0)
		if ((waitpid(-1, &wstat, 0) == -1)
			|| WEXITSTATUS(wstat) == EXIT_FAILURE)
			++cfails;
	if (cfails != 0)
		return (-1);
	return (0);
}

static int	exc_exec_cmd(t_cmd *cmd, t_core *core)
{
	int	is_bltn;
	int	check_rtv;

	if ((*cmd->argv == NULL && ft_eprintf(ERR_CMD, NULL))
		|| (**cmd->argv == 0 && ft_eprintf(ERR_ECMD, **cmd->argv)))
	{
		psr_cleanup_cmds(core->flags, core);
		exit(EXIT_SUCCESS);
	}
	if (fmgr_dup2(cmd->ifd, 0) == -1
		|| fmgr_dup2(cmd->ofd, 1) == -1
		|| psr_cleanup_cmds(FLG_REDS, core) != 1)
		return (-1);
	is_bltn = exc_is_builtin(*cmd->argv);
	// if (is_bltn && exc_builtins(cmd->argv, envp) == -1) // arrays to pointer to funct?
	// 	return (-1);
	check_rtv = 0;
	if (is_bltn == false)
		check_rtv = exc_check_path(cmd->argv, core->envp);
	if (check_rtv == -1
		|| (check_rtv == 1 && execve(*cmd->argv, cmd->argv, core->envp) == -1))
		return (-1);
	return (0);
}

// static int	exc_spawn_chld(int i, t_cmd *cmd, t_core *core)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
// 	{
// 		exc_wait_cmds(i);
// 		return (-1);
// 	}
// 	if (pid == 0 && exc_exec_cmd(cmd, core) == -1)
// 	{
// 		psr_cleanup_cmds(core->flags, core);
// 		printf("Exit failure\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	return (0);
// }

// 5/10 - The logic is completely faulty
// = start by testing ""
int	exc_exec_cmds(t_core *core)
{
	t_cmd	*cmd;
	int		i;
	pid_t	pid;

	i = -1;
	pid = 0;
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		if (cmd->xready == false)
			continue ;
		pid = fork();
		if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		{
			exc_wait_cmds(core->cmd_xrdy);
			return (-1);
		}
		if (pid == 0 && exc_exec_cmd(cmd, core) == -1)
		{
			psr_cleanup_cmds(core->flags, core);
			exit(EXIT_FAILURE);
		}
	}
	if (pid != 0 && utl_cleanup((FLG_CMDS | FLG_REDS), core)
		&& exc_wait_cmds(core->cmd_xrdy) == -1)
		return (-1);
	return (0);
}
