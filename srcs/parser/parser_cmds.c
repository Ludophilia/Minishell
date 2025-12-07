/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:57:18 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/07 22:43:25 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_tok_has_quotes(t_tok *tok)
{
	int	i;

	i = -1;
	while (++i < tok->len)
		if (tok->start[i] == '\'' || tok->start[i] == '\"')
			return (1);
	return (0);
}


static int	psr_count_args(t_tok *tok)
{
	int	size;

	size = 0;
	while (tok->type == TOK_WORD || psr_isred(tok))
	{
		if (psr_isred(tok))
			tok += 2;
		else
		{
			size++;
			tok++;
		}
	}
	return (size);
}


// 9/12 - Please improve this. 42 lines!!
// 
int	psr_add_cmd(t_cnt *c, t_tok *toks, t_cmd *cmd, t_core *core)
{
	int		len;
	int		argc;
	char	*word;
	t_red	*red;
	t_tok	*tok;

	len = psr_count_args(toks + c->i);
	cmd->argv = ft_calloc(len + 1, sizeof(char *));
	if (cmd->argv == NULL)
		return (-1);

	argc = 0;
	
	red = cmd->reds;

	tok = toks + c->i;

	while (tok->type == TOK_WORD || psr_isred(tok))
	{
		if (psr_isred(tok))
		{
			red->type = tok->type;
			red->word = psr_create_word(tok + 1, red->type, core);
			if (red->word == NULL)
				return (-1);
			red++;
			c->i += 2; // Consume the tokens
		}
		else
		{
			word = psr_create_word(tok, TOK_WORD, core);
			if (word == NULL)
				return (-1);
			if (*word || psr_tok_has_quotes(tok))
				cmd->argv[argc++] = word;
			else
				free(word);
			c->i++; // Consume the tokens
		}
		tok = toks + c->i;
	}
	return ((cmd->argc = argc), 0);
}
