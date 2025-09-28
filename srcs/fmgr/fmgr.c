/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:04:49 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/28 23:35:42 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fmgr_set_ifds(t_cmd *cmd)
{
	t_red	*ireds;
	int		j;

	j = -1;
	ireds = cmd->ireds;
	while (ireds[++j].type != TOK_EOL)
	{
		if (ireds[j].type == TOK_IRED
			&& fmgr_set_red(0, cmd->ifds, FL_IRED, ireds + j) == -1)
			return (-1);
		else if (ireds[j].type == TOK_IRED_HD
			&& fmgr_set_hdocs(cmd->ifds, ireds + j) == -1)
			return (-1);
	}
	return (0);
}

static int	fmgr_set_ofds(t_cmd *cmd)
{
	t_red	*oreds;
	int		j;

	j = -1;
	oreds = cmd->oreds;
	while (oreds[++j].type != TOK_EOL)
	{
		if (oreds[j].type == TOK_ORED
			&& fmgr_set_red(1, cmd->ofds, FL_ORED, oreds + j) == -1)
			return (-1);
		else if (oreds[j].type == TOK_ORED_AP
			&& fmgr_set_red(1, cmd->ofds, FL_ORED_AP, oreds + j) == -1)
			return (-1);
	}
	return (0);
}

int	fmgr_set_reds(t_core *core)
{
	int		i;
	t_cmd	*cmd;
	int		cmd_nbr;

	cmd_nbr = core->cmd_pmax + 1;
	i = -1;
	while (++i < cmd_nbr)
	{
		cmd = core->cmds + i;
		if (cmd_nbr > 1 && fmgr_set_pipe(i, core->cmd_pmax, cmd) == -1)
			return (-1);
		if (fmgr_set_ifds(cmd) == -1
			|| fmgr_set_ofds(cmd) == -1)
			continue ;
		cmd->xready = true;
	}
	return (0);
}
