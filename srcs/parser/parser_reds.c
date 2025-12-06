/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_reds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:27:28 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/06 20:12:24 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_add_red(t_red *reds, t_tok **tok, t_core *core)
{
	t_red	*red;
	int		pos;

	pos = 0;
	while (reds[pos].type != TOK_EOL)
		pos++;
	red = reds + pos;
	red->type = (*tok)->type;
	red->word = psr_create_word(*tok + 1, red->type, core);
	if (red->word == NULL)
		return (-1);
	return (*tok += 1, 0);
}

int	psr_add_reds(t_tok *tok, t_cmd *cmd, t_core *core)
{
	while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
	{
		if (psr_isred(tok) && psr_add_red(cmd->reds, &tok, core) == -1)
			return (-1);
		tok++;
	}
	return (0);
}
