/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:27:28 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/28 22:44:08 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_add_red(t_red *reds, t_tok **tok)
{
	t_red	*red;
	int		pos;

	pos = 0;
	while (reds[pos].type != TOK_EOL)
		pos++;
	red = reds + pos;
	red->type = (*tok)->type;
	red->word = psr_create_word(*tok + 1, red->type);
	if (red->word == NULL)
		return (-1);
	return (*tok += 1, 0);
}

int	psr_add_reds(t_tok *tok, t_cmd *cmd)
{
	while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
	{
		if ((psr_is_ired(tok) && psr_add_red(cmd->ireds, &tok) == -1)
			|| (psr_is_ored(tok) && psr_add_red(cmd->oreds, &tok) == -1))
			return (-1);
		tok++;
	}
	return (0);
}
