/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 13:37:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/20 00:07:32 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_redtok_check(t_tok *tok, t_tok *next)
{
	if (psr_isored(tok) != true && psr_isired(tok) != true)
		return (0);
	if (next->type == TOK_WORD)
	{
		psr_synterr(next);
		return (-1);
	}
	return (0);
}

static int	psr_binaop_check(int pos, t_tok *tok, t_tok *next)
{
	if (psr_isop(tok) != true)
		return (0);
	if (pos == 0)
	{
		psr_synterr(tok);
		return (-1);
	}
	if (psr_isop(next)
		|| next->type == TOK_EOL
		|| next->type == TOK_SUBC)
	{
		psr_synterr(next);
		return (-1);
	}
	return (0);
}

static int	psr_wtok_check(t_tok *tok)
{
	int		i;
	int		quoted;

	if (tok->type != TOK_WORD)
		return (0);
	i = -1;
	quoted = 0;
	while (++i < tok->len)
	{
		if (!quoted && lex_is_quote(tok->start + i))
			quoted = tok->start[i];
		else if (quoted && tok->start[i] == quoted)
			quoted = 0;
	}
	if (quoted != 0 && psr_synterr(&(t_tok){TOK_EOL}))
		return (-1);
	return (0);
}

static int	psr_subtok_check(int pos, int *opn, t_tok *tok, t_tok *next)
{
	if (tok->type != TOK_SUBO && tok->type != TOK_SUBC)
		return (0);
	if (tok->type == TOK_SUBO)
		(*opn)++;
	if (tok->type == TOK_SUBC)
		(*opn)--;
	if (tok->type == TOK_SUBC && (pos == 0 || *opn < 0))
	{
		psr_synterr(tok);
		return (-1);
	}
	if ((tok->type == TOK_SUBC
			&& (psr_isop(next) || next->type == TOK_SUBC))
		|| (tok->type == TOK_SUBC
			&& (next->type == TOK_WORD || next->type == TOK_SUBO)))
	{
		psr_synterr(next);
		return (-1);
	}
	return (0);
}

int	psr_error_check(t_tok *toks, t_core *core)
{
	t_tok	*tok;
	int		i;
	int		opn;

	opn = 0;
	i = 0;
	tok = toks + i;
	while (tok->type != TOK_EOL)
	{
		if (psr_redtok_check(tok, tok + 1) == -1
			|| psr_binaop_check(i, tok, tok + 1) == -1
			|| psr_wtok_check(tok) == -1
			|| psr_subtok_check(i, &opn, tok, tok + 1) == -1)
			return (core->exit = 2, -1);
		tok = toks + ++i;
	}
	if (opn != 0)
		return (core->exit = 2, psr_synterr(tok), -1);
	return (0);
}
