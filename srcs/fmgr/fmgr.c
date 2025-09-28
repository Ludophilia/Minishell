/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:04:49 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/28 21:35:42 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fmgr_set_pipe(int pos, int pmax, t_cmd *cmd)
{
	t_cmd	*cmd;

	if (pmax == 0)
		return (0);
	if (pos != pmax && fmgr_pipe(cmd->ofds) == -1)
		return (-1); // close_all fds that has been opened so far ??
	if (pos != 0)
	{
		cmd->ifds[0] = (cmd - 1)->ofds[0];
		cmd->ifds[1] = (cmd - 1)->ofds[1];
	}
	return (0);
}

static int	fmgr_set_red(int *fds, int openflags, t_red *red)
{
	int	fd;

	fd = fmgr_open(red->word, openflags, FL_PRMS);
	if (fd == -1)
		return (-1); // Cleanup?
	if (fds[0] > 2 && fmgr_close(0, fds) == -1)
		return (fmgr_close(0, &fd), -1);
	fds[0] = fd;
	return (0);
}

static int	fmgr_set_ireds(t_cmd *cmd)
{
	t_red	*ireds;
	int		j;

	j = -1;
	ireds = cmd->ireds;
	while (ireds[++j].type != TOK_EOL)
	{
		if (ireds[j].type == TOK_IRED
			&& fmgr_set_red(cmd->ifds, FL_IRED, ireds + j) == -1)
			return (-1); // Error management ??
		else if (ireds[j].type == TOK_IRED_HD
			&& fmgr_set_hdocs(cmd->ifds, ireds + j) == -1)
			return (-1); // Error management ??
	}
	return (0);
}

static int	fmgr_set_oreds(t_cmd *cmd)
{
	t_red	*oreds;
	int		j;

	j = -1;
	oreds = cmd->oreds;
	while (oreds[++j].type != TOK_EOL)
	{
		if (oreds[j].type == TOK_ORED
			&& fmgr_set_red(cmd->ofds, FL_ORED, oreds + j) == -1)
			return (-1); // Error management ??
		else if (oreds[j].type == TOK_ORED_AP
			&& fmgr_set_red(cmd->ofds, FL_ORED_AP, oreds + j) == -1)
			return (-1); // Error management ??
	}
	return (0);
}

// 28/09 - Now error manage all of this.
int	fmgr_set_reds(t_core *core)
{
	int		i;
	t_cmd	*cmd;

	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		if (fmgr_set_pipe(i, core->cmd_pmax, cmd) == -1)
			return (-1);
		if (fmgr_set_ireds(cmd) == -1
			|| fmgr_set_oreds(cmd) == -1)
			continue ; // Error management? An error does not mean the end 
			// of the pipeline
		cmd->xready = true;
	}
	return (0);
}
