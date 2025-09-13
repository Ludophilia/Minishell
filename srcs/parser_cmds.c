/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:57:18 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/13 18:59:42 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_count_argc(t_tok *token)
{
	int	size;

	size = 0;
	while (token->type != TOK_PIPE && token->type != TOK_EOL)
	{
		if (psr_is_ored(token) || psr_is_ired(token))
			token += 2;
		else
		{
			size++;
			token++;
		}
	}
	return (size);
}

int	psr_add_cmd(t_tok *token, t_cmd *cmd)
{
	int		size;
	int		pos;

	size = psr_count_argc(token);
	cmd->argv = malloc((size + 1) * sizeof(char *));
	if (cmd->argv == NULL)
		return (-1);
	pos = 0;
	while (token->type != TOK_PIPE && token->type != TOK_EOL)
	{
		if (psr_is_ored(token) || psr_is_ired(token))
		{
			token += 2;
			continue ;
		}
		cmd->argv[pos] = psr_create_word(token);
		if (cmd->argv[pos++] == NULL) // Where are the routines for destroying data in that context.
			return (-1);
		token++;
	}
	cmd->argv[size] = NULL;
	return (0);
}
