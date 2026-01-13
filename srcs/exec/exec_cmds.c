/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:54:02 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/13 18:34:14 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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



int	exc_exec_scmd(int ifd, int ofd, t_astn *root, t_core *core)
{
	t_cmd	*cmd;
	int		status;

	cmd = root->content;
	ft_eprintf("[%i] About to close extra pipes in simplecmd except (%i, %i)\n",
			getpid(), ifd, ofd);


	// if (fmgr_close_extra_pipes(ifd, ofd, core) == -1)
	// 	return (EX_F);
	if (fmgr_process_reds(&ifd, &ofd, cmd, core) == -1)
		return (core->exit);


	if (*cmd->argv == NULL)
		return (EX_S);
	if (exc_is_builtin(cmd, &status))
		return (core->exit = exc_exec_builtin(status, cmd, core));
	status = exc_exec_prg(cmd, core);
	if (status == -1)
		return (EX_F);
	return (EX_S);
}
