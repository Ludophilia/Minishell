/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:57:18 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/19 13:48:51 by ntahri           ###   ########.fr       */
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

/* int	psr_add_cmd(t_tok *tok, t_cmd *cmd, t_core *core)
{
	int		pos;

	cmd->argc = psr_count_args(tok);
	cmd->argv = ft_calloc(cmd->argc + 1, sizeof(char *));
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
		cmd->argv[pos] = psr_create_word(tok, TOK_WORD, core);
		if (cmd->argv[pos++] == NULL)
			return (-1);
		tok++;
	}
	return (0);
} */

static int	psr_tok_has_quotes(t_tok *tok)
{
	int	i;

	i = 0;
	while (i < tok->len)
		if (tok->start[i] == '\'' || tok->start[i++] == '\"')
			return (1);
	return (0);
}

int	psr_add_cmd(t_tok *tok, t_cmd *cmd, t_core *core)
{
	int		pos;
	char	*word;

	cmd->argc = psr_count_args(tok);
	cmd->argv = ft_calloc(cmd->argc + 1, sizeof(char *));
	if (!cmd->argv)
		return (-1);
	pos = 0;
	while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
	{
		if (psr_is_ored(tok) || psr_is_ired(tok))
			tok += 2;
		else
		{
			word = psr_create_word(tok, TOK_WORD, core);
			if (!word)
				return (-1);
			if (*word || psr_tok_has_quotes(tok))
				cmd->argv[pos++] = word;
			else
				free(word);
			tok++;
		}
	}
	return ((cmd->argc = pos), 0);
}

