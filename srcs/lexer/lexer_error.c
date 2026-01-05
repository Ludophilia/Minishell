/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 13:37:44 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/05 02:24:07 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lex_redtok_check(t_tok *tok, t_tok *next)
{
	if (lex_isred(tok) != true)
		return (0);
	if (next->type != TOK_WORD)
	{
		lex_synterr(next->type, next);
		return (-1);
	}
	return (0);
}

static int	lex_binaop_check(int pos, t_tok *tok, t_tok *next)
{
	if (lex_isop(tok) != true)
		return (0);
	if (pos == 0)
	{
		lex_synterr(tok->type, tok);
		return (-1);
	}
	if (lex_isop(next)
		|| next->type == TOK_EOL
		|| next->type == TOK_SUBC)
	{
		lex_synterr(next->type, next);
		return (-1);
	}
	return (0);
}

static int	lex_wtok_check(t_tok *tok, t_tok *next)
{
	int		i;
	int		quoted;

	if (tok->type != TOK_WORD)
		return (0);
	if (next->type == TOK_SUBO && lex_synterr(next->type, next))
		return (-1);
	i = -1;
	quoted = 0;
	while (++i < tok->len)
	{
		if (!quoted && lex_is_quote(tok->start + i))
			quoted = tok->start[i];
		else if (quoted && tok->start[i] == quoted)
			quoted = 0;
	}
	if (quoted != 0 && lex_synterr(TOK_EOL, NULL))
		return (-1);
	return (0);
}

int	lex_error_check(t_tok *toks, t_core *core)
{
	t_tok	*tok;
	int		i;
	int		opn;

	opn = 0;
	i = 0;
	tok = toks + i;
	while (tok->type != TOK_EOL)
	{
		if (lex_redtok_check(tok, tok + 1) == -1
			|| lex_binaop_check(i, tok, tok + 1) == -1
			|| lex_wtok_check(tok, tok + 1) == -1
			|| lex_subtok_check(&i, &opn, tok, tok + 1) == -1)
			return (core->exit = 2, -1);
		tok = toks + ++i;
	}
	if (opn != 0)
		return (core->exit = 2, lex_synterr(tok->type, tok), -1);
	return (0);
}
