/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:04:49 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/07 17:11:37 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fmgr_set_xfds(t_cmd *cmd)
{
	t_red	*red;
	int		j;

	j = -1;
	red = cmd->reds + ++j;
	while (red->type != TOK_EOL)
	{
		if ((red->type == TOK_IRED
				&& fmgr_set_red(&cmd->ifd, FL_IRED, red) == -1)
			|| (red->type == TOK_ORED
				&& fmgr_set_red(&cmd->ofd, FL_ORED, red) == -1)
			|| (red->type == TOK_IRED_HD
				&& fmgr_set_hdocs(&cmd->ifd, red) == -1)
			|| (red->type == TOK_ORED_AP
				&& fmgr_set_red(&cmd->ofd, FL_ORED_AP, red) == -1))
			return (-1);
		red = cmd->reds + ++j;
	}
	return (0);
}

int	fmgr_set_reds(t_core *core)
{
	t_cmd	*cmd;
	int		nbr;
	int		i;

	i = -1;
	nbr = core->cmd_pmax + 1;
	while (++i < nbr)
	{
		cmd = core->cmds + i;
		if (nbr > 1 && fmgr_set_pipe(i, core->cmd_pmax, cmd) == -1)
			return (-1);
		if (fmgr_set_xfds(cmd) == -1)
			continue ;
		if (cmd->argv != NULL && *cmd->argv != NULL)
		{
			cmd->xready = true;
			core->cmd_xrdy++;
		}
	}
	return (0);
}
