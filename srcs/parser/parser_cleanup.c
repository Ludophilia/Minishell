/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 22:43:08 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/28 23:15:18 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_cleanup_cmd(t_cmd *cmd)
{
	int	j;

	if (cmd->argv == NULL)
		return (1);
	j = 0;
	while (cmd->argv[j])
		free(cmd->argv[j++]);
	free(cmd->argv);
	cmd->argv = NULL;
	return (1);
}

static int	psr_cleanup_red(int is_inpt, t_red *red, int *fds)
{
	while (red->type != TOK_EOL)
	{
		red->type = TOK_EOL;
		free(red->word);
		red->word = NULL;
		red++;
	}
	(void)((is_inpt && fmgr_close(0, fds)) || fmgr_close(1, fds)); // remove void
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
		psr_cleanup_red(true, cmd->ireds, cmd->ifds);
		psr_cleanup_red(false, cmd->oreds, cmd->ofds);
	}
	core->flags |= ~FLG_CMDS;
	core->cmd_pmax = 0;
	return (1);
}
