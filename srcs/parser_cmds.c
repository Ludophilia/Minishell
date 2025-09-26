/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:57:18 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/26 12:07:11 by jegerman         ###   ########.fr       */
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

int	psr_cleanup_cmds(t_core *core)
{
	t_cmd	*cmd;
	int		i;

	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		psr_cleanup_cmd(cmd);
		psr_cleanup_red(cmd->ireds, cmd->ifds);
		psr_cleanup_red(cmd->oreds, cmd->ofds);
	}
	core->flags |= ~FLG_CMDS;
	core->cmd_pmax = 0;
	return (1);
}

static int	psr_count_args(t_tok *tok)
{
	int	size;

	size = 0;
	while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
	{
		if (psr_is_ored(tok) || psr_is_ired(tok))
			tok += 2;
		else
			(size++, tok++);
	}
	return (size);
}

int	psr_add_cmd(t_tok *tok, t_cmd *cmd)
{
	int		size;
	int		pos;

	size = psr_count_args(tok);
	cmd->argv = ft_calloc(size + 1, sizeof(char *));
	if (cmd->argv == NULL)
		return (-1);
	pos = 0;
	while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
	{
		if (psr_is_ored(tok) || psr_is_ired(tok))
		{
			tok += 2;
			continue ;
		}
		cmd->argv[pos] = psr_create_word(tok, TOK_WORD);
		if (cmd->argv[pos++] == NULL)
			return (-1);
		tok++;
	}
	return (0);
}
