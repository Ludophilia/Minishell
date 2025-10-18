/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/17 23:34:32 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exc_wait_cmds(t_core *core)
{
	t_cmd	*cmd;
	int		wstat;
	int		i;

	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		if (cmd->xready == false)
			continue ;
		if (waitpid(cmd->pid, &wstat, 0) == -1)
			return (-1);
		if (WIFEXITED(wstat))
			core->exit = WEXITSTATUS(wstat);
		else if (WIFSIGNALED(wstat))
			core->exit = 128 + WTERMSIG(wstat);
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
		|| fmgr_dup2(cmd->ofd, 1) == -1)
		return (-1);
	if (exc_if_builtin(cmd, core) == true)
		return (0);
	if (utl_cleanup(FLG_REDS, core) != 1)
		return (-1);
	envp = env_get_envp(core->env);
	if (envp == NULL)
		return (-1);
	chkrv = exc_check_path(cmd->argv, envp);
	if (chkrv == -1
		|| (chkrv > 0 && execve(*cmd->argv, cmd->argv, envp) == -1))
		return (utl_free_strs(0, envp), -1);
	else if (chkrv == -2)
		(utl_free_strs(0, envp), exit(127));
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

int	exc_exec_cmds(t_core *core)
{
	pid_t	pid;
	int		i;

	pid = 0;
	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		if (core->cmds[i].xready == false
			|| (core->cmd_pmax == 0 && exc_if_builtin(core->cmds + i, core)))
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
