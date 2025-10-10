/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/10 21:50:26 by jegerman         ###   ########.fr       */
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
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		if (cmd->xready == false)
			continue ;
		if (waitpid(cmd->pid, &wstat, 0) == -1) // DANGER
			return (-1);
		core->exitv = WEXITSTATUS(wstat); // One way...
		g_exit_status = WEXITSTATUS(wstat); // ...or the other.
	}
	return (0);
}

// 7/10 - May have to split it one more time with builtins and stuff...
static int	exc_exec_cmd(t_cmd *cmd, t_core *core)
{
	int	is_bltn;
	int	chk_rv;

	chk_rv = 0;
	if ((*cmd->argv == NULL && ft_eprintf(ERR_CMD, NULL))
		|| (**cmd->argv == 0 && ft_eprintf(ERR_ECMD, **cmd->argv))) 	// if (!cmd || !cmd->argv || !*cmd->argv) // Why?
		return (0);

	if (fmgr_dup2(cmd->ifd, 0) == -1 || fmgr_dup2(cmd->ofd, 1) == -1
		|| psr_cleanup_cmds(FLG_REDS, core) != 1)
		return (-1);
	is_bltn = exc_is_builtin(*cmd->argv);
	// if (is_bltn >= 0) // 10/10 - Logic seems to have changed
	// 	return (g_exit_status = exc_exec_builtin(core, cmd, STDOUT_FILENO));
	if (is_bltn == false)
		chk_rv = exc_check_path(cmd->argv, core->envp);
	if (chk_rv == -1
		|| (chk_rv == 1 && execve(*cmd->argv, cmd->argv, core->envp) == -1))
		return (-1);
	return (0);
}

static int	exc_init_subsh(int i, pid_t *pid, t_core *core)
{
	t_cmd	*cmd;
	int		exit_val;
	int		exec_rv;

	cmd = core->cmds + i;
	*pid = fork();
	if (*pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (*pid > 0)
		return (cmd->pid = *pid, 0);
	// sig_init_child(); // 10/10
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
		// 10/10 - The "outside of a pipeline" builtin case
		// if (exc_is_builtin(core->cmds[i].argv[0]) >= 0 && core->cmd_pmax == 0)
		// {
		// 	g_exit_status = exc_exec_builtin(core, &core->cmds[i], core->cmds[i].ofd);
		// 	continue ; // Really? No cleanup?
		// }	
		if (exc_init_subsh(i, &pid, core) == -1
			&& utl_cleanup(FLG_REDS, core)
			&& exc_wait_cmds(core))
			return (-1);
	}
	if (pid > 0
		&& utl_cleanup((FLG_REDS), core)
		&& exc_wait_cmds(core) == -1)
		return (-1);
	// sig_init_prompt(); // 10/10 - What is it? Why?
	return (0);
}
