/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/07 16:53:26 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 6/10 = Improve waiting logic so that < /dev/urandom tail does not fail
// ^C make the child defunct. The SIGINT processing logic need to be 
// implemented.
static int	exc_wait_cmds(int i)
{
	int	wstat;
	int	cfails;

	cfails = 0;
	while (--i >= 0)
		if ((waitpid(-1, &wstat, 0) == -1) // the pipe order shld be respected.
			|| WEXITSTATUS(wstat) == EXIT_FAILURE)
			++cfails;
	if (cfails != 0)
		return (0); // 6/10 = Error code from "waited" command should be here.
	return (0);
}

// 7/10 - May have to split it one more time with builtins and stuff...
static int	exc_exec_cmd(t_cmd *cmd, t_core *core)
{
	int	is_bltn;
	int	chk_rv;

	chk_rv = 0;
	if ((*cmd->argv == NULL && ft_eprintf(ERR_CMD, NULL))
		|| (**cmd->argv == 0 && ft_eprintf(ERR_ECMD, **cmd->argv)))
		return (0);
	if (fmgr_dup2(cmd->ifd, 0) == -1 || fmgr_dup2(cmd->ofd, 1) == -1
		|| psr_cleanup_cmds(FLG_REDS, core) != 1)
		return (-1);
	is_bltn = exc_is_builtin(*cmd->argv);
	if (is_bltn == false)
		chk_rv = exc_check_path(cmd->argv, core->envp);
	if (chk_rv == -1
		|| (chk_rv == 1 && execve(*cmd->argv, cmd->argv, core->envp) == -1))
		return (-1);
	return (0);
}

static int	exc_init_subsh(int i, pid_t *pid, t_core *core)
{
	int	exit_val;
	int	exec_rv;

	*pid = fork();
	if (*pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (*pid > 0)
		return (0);
	exit_val = EXIT_SUCCESS;
	exec_rv = exc_exec_cmd(core->cmds + i, core);
	psr_cleanup_cmds(core->flags, core);
	if (exec_rv == -1)
		exit_val = EXIT_FAILURE;
	exit(exit_val);
}

int	exc_exec_cmds(t_core *core)
{
	pid_t	pid;
	int		i;

	pid = 0;
	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		if (core->cmds[i].xready == false)
			continue ;
		if (exc_init_subsh(i, &pid, core) == -1
			&& exc_wait_cmds(core->cmd_xrdy))
			return (-1);
	}
	if (pid > 0
		&& utl_cleanup((FLG_CMDS | FLG_REDS), core)
		&& exc_wait_cmds(core->cmd_xrdy) == -1)
		return (-1);
	return (0);
}
