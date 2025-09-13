/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:27:28 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/13 18:59:12 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_is_ired(t_tok *token)
{
	return (token->type == TOK_IRED || token->type == TOK_IRED_HD);
}

int	psr_is_ored(t_tok *token)
{
	return (token->type == TOK_ORED || token->type == TOK_ORED_AP);
}

static int	psr_add_red(t_red *reds, int *pos, t_tok **token)
{
	t_red	*red;

	red = reds + *pos;
	red->type = (*token)->type;
	red->word = psr_create_word(token[1]);
	if (red->word == NULL)
		return (-1);
	*pos += 1;
	*token += 1;
	return (0);
}

int	psr_add_reds(t_tok *token, t_cmd *cmd)
{
	while (token->type != TOK_PIPE && token->type != TOK_EOL)
	{
		if (psr_is_ired(token))
			if (psr_add_red(cmd->ireds, &cmd->ilen, token) == -1)
				return (-1); // Error handling?
		if (psr_is_ored(token))
			if (psr_add_red(cmd->oreds, &cmd->olen, token) == -1)
				return (-1); // Error handling?
		token++;
	}
	return (0);
}
