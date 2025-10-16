/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/16 22:11:24 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 6/10 = Improve waiting logic so that < /dev/urandom tail does not fail
// ^C make the child defunct. The SIGINT processing logic need to be 
// implemented.

// 10/10 - Merge the two versions.
// static int	exc_wait_cmds(int count)
// {
// 	int	wstat;
// 	int	cfails;

// 	cfails = 0;
// 	while (--count >= 0)
// 	{
// 		if (waitpid(-1, &wstat, 0) == -1)
// 			continue ;
// 		if (WIFEXITED(wstat))
// 			g_exit_status = WEXITSTATUS(wstat);
// 		else if (WIFSIGNALED(wstat))
// 			g_exit_status = 128 + WTERMSIG(wstat);
// 		if (g_exit_status != 0)
// 			cfails++;
// 	}
// 	return (cfails != 0);
// }

static int	exc_wait_cmds(t_core *core)
{
	t_cmd	*cmd;
	int		wstat;
	int		i;

	i = -1;
	// 16/10 - Signals are still not implemented
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		if (cmd->xready == false)
			continue ;
		if (waitpid(cmd->pid, &wstat, 0) == -1)
			return (-1);
		core->exit = WEXITSTATUS(wstat);
	}
	return (0);
}

static int	exc_exec_cmd(t_cmd *cmd, t_core *core)
{
	char	**envp;
	int		chkrv;

	if ((*cmd->argv == NULL && ft_eprintf(ERR_CMD, NULL))
		|| (**cmd->argv == 0 && ft_eprintf(ERR_ECMD, **cmd->argv)))
		return (0);
	if (fmgr_dup2(cmd->ifd, 0) == -1
		|| fmgr_dup2(cmd->ofd, 1) == -1
		|| utl_cleanup(FLG_REDS, core) != 1)
		return (-1);
	if (exc_if_builtin(cmd, core) == true)
		return (0);
	envp = env_get_envp(core->env);
	if (envp == NULL)
		return (-1);
	chkrv = exc_check_path(cmd->argv, envp);
	if (chkrv == -1
		|| (chkrv > 0 && execve(*cmd->argv, cmd->argv, envp) == -1))
	{
		utl_free_strs(0, envp);
		return (-1);
	}
	return (utl_free_strs(0, envp), 0);
}

static int	exc_init_subsh(int i, pid_t *pid, t_core *core)
{
	t_cmd	*cmd;
	int		exec_rv;

	cmd = core->cmds + i;
	*pid = fork();
	if (*pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (*pid > 0)
		return (cmd->pid = *pid, 0);
	if (sig_init_child() == -1 && utl_cleanup(core->flags | FLG_ENV, core))
		exit(EX_FAIL);
	exec_rv = exc_exec_cmd(core->cmds + i, core);
	utl_cleanup(core->flags | FLG_ENV, core);
	if (exec_rv == -1)
		exit(EX_FAIL);
	exit(EX_SUCC);
}

// 16/10 - Here we are...
int	exc_exec_cmds(t_core *core)
{
	pid_t	pid;
	int		i;

	pid = 0;
	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		if (!core->cmds[i].xready
			|| (!core->cmd_pmax && exc_if_builtin(core->cmds + i, core) != -1))
			continue ;
		if (exc_init_subsh(i, &pid, core) == -1
			&& utl_cleanup(FLG_REDS, core)
			&& exc_wait_cmds(core))
			return (-1);
	}
	if (pid > 0
		&& utl_cleanup(FLG_REDS, core)
		&& exc_wait_cmds(core) == -1)
		return (-1);
	return (0);
}
