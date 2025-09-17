/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:27:28 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/17 23:23:42 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int psr_init_reds(t_cmd *cmd)
{
	int	i;

	i = -1;
	while (++i < RED_MAX)
	{
		(cmd->ireds[i]).type = TOK_EOL;
		(cmd->oreds[i]).type = TOK_EOL;
	}
	return (0);
}

static int	psr_add_red(t_red *reds, t_tok **tok)
{
	t_red	*red;
	int		pos;

	pos = 0;
	while (reds[pos].type != TOK_EOL)
		pos++;
	red = reds + pos;
	red->type = (*tok)->type;
	red->word = psr_create_word(tok[1], (*tok)->type);
	if (red->word == NULL)
		return (-1);
	return (*tok += 1, 0);
}

int	psr_add_reds(t_tok *tok, t_cmd *cmd)
{
	psr_init_reds(cmd);
	while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
	{
		if (psr_is_ired(tok) && psr_add_red(cmd->ireds, &tok) == -1)
			return (-1); // Error handling?
		if (psr_is_ored(tok) && psr_add_red(cmd->oreds, &tok) == -1)
			return (-1); // Error handling?
		tok++;
	}
	return (0);
}
