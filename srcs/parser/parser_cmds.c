/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:57:18 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/09 16:10:04 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_count_args(t_tok *tok)
{
	int	size;

	size = 0;
	while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
	{
		if (psr_is_ored(tok) || psr_is_ired(tok))
			tok += 2;
		else
			(void)(size++, tok++);
	}
	return (size);
}

int	psr_add_cmd(t_tok *tok, t_cmd *cmd)
{
	int	size;
	int	pos;

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
