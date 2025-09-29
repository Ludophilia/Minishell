/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 22:43:08 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/29 19:34:15 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_cleanup_cmd(t_cmd *cmd)
{
	int	j;

	cmd->xready = 0;
	if (cmd->argv == NULL)
		return (1);
	j = 0;
	while (cmd->argv[j])
		free(cmd->argv[j++]);
	free(cmd->argv);
	cmd->argv = NULL;
	// pid?
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
	return (0);
}

int	psr_cleanup_cmds(t_core *core)
{
	t_cmd	*cmd;
	int		i;

	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		psr_cleanup_cmd(cmd);
		psr_cleanup_red(cmd);
	}
	core->flags |= ~FLG_CMDS;
	core->cmd_pmax = 0;
	return (1);
}
