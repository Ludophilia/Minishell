/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:54:02 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/10 15:19:59 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exc_close_pipes(int ifd, int ofd, t_core *core)
{
	int		i;
	int		*pipe;
	int		fails;

	fails = 0;
	i = -1;
	while (core->stash[++i])
	{
		pipe = (core->stash[i])->content;
		if ((pipe[0] != ifd
				&& pipe[0] != ofd
				&& fmgr_close(pipe) == -1)
			|| (pipe[1] != ifd
				&& pipe[1] != ofd
				&& fmgr_close(pipe + 1) == -1))
			fails++;
	}
	if (fails)
		return (-1);
	return (0);
}

static int	exc_exec_prg(t_cmd *cmd, t_core *core)
{
	int	status;

	if (sig_init_child() == -1
		|| fmgr_dup2(cmd->ifd, 0) == -1
		|| fmgr_dup2(cmd->ofd, 1) == -1
		|| env_get_envp(core->env, core) == NULL)
		return (-1);
	status = exc_check_path(cmd->argv, core->envp, core);
	if (status == -1)
		return (-1);
	if (status == false
		&& utl_exit(core->exit, core))
		return (0);
	if (status == true
		&& execve(*cmd->argv, cmd->argv, core->envp) == -1)
		return (-1);
	return (0);
}

int	exc_process_reds(int *ifd, int *ofd, t_astn *root, t_core *core)
{
	t_cmd	*cmd;

	cmd = root->content;
	cmd->ifd = *ifd;
	cmd->ofd = *ofd;
	if (fmgr_set_xfds(cmd, core) == -1)
		return (-1);
	*ifd = cmd->ifd;
	*ofd = cmd->ofd;
	return (0);
}

int	exc_exec_scmd(int ifd, int ofd, t_astn *root, t_core *core)
{
	t_cmd	*cmd;
	int		status;

	cmd = root->content;
	if (exc_close_pipes(ifd, ofd, core) == -1)
		return (EX_F);
	if (exc_process_reds(&ifd, &ofd, root, core) == -1)
		return (core->exit);
	if (*cmd->argv == NULL)
		return (EX_S);
	if (exc_if_builtin(cmd, core))
		return (core->exit);
	status = exc_exec_prg(cmd, core);
	if (status == -1)
		return (EX_F);
	return (EX_S);
}
