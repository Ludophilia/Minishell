/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 22:43:08 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/03 22:56:16 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_cleanup_cmd(t_cmd *cmd)
{
	cmd->xready = 0;
	if (cmd->argv == NULL)
		return (1);
	utl_free_strs(0, cmd->argv);
	cmd->argv = NULL;
	return (1);
}

static int	psr_cleanup_red(t_cmd *cmd)
{
	t_red *red;

	red = cmd->reds;
	while (red->type != TOK_EOL)
	{
		red->type = TOK_EOL;
		free(red->word);
		red->word = NULL;
		red++;
	}
	fmgr_close(&cmd->ifd);
	fmgr_close(&cmd->ofd);
	return (1);
}

int	psr_cleanup_cmds(t_cflg flags, t_core *core)
{
	t_cmd	*cmd;
	int		i;

	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		if (flags & FLG_CMDS)
			psr_cleanup_cmd(cmd);
		if (flags & FLG_REDS)
			psr_cleanup_red(cmd);
	}
	if (flags & FLG_CMDS)
		core->flags |= ~FLG_CMDS;
	if (flags & FLG_REDS)
		core->flags |= ~FLG_REDS;
	core->cmd_pmax = 0;
	return (1);
}
