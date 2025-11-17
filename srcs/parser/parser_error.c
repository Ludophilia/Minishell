/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 13:37:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/17 21:03:18 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_redtok_check(t_tok *tok)
{
	if (psr_isored(tok) == false && psr_isired(tok) == false)
		return (0);
	if (psr_istok(TOK_WORD, tok + 1) == false)
	{
		psr_synterr(tok + 1);
		return (-1);
	}
	return (0);
}

static int	psr_binaop_check(int pos, t_tok *tok)
{
	if (psr_isop(tok) != true)
		return (0);
	if (pos == 0)
	{
		psr_synterr(tok);
		return (-1);
	}
	if (psr_isop(tok + 1)
		|| psr_istok(TOK_EOL, tok + 1)
		|| psr_istok(TOK_SUBC, tok + 1))
	{
		psr_synterr(tok + 1);
		return (-1);
	}
	return (0);
}

static int	psr_wtok_check(t_tok *tok)
{
	int		i;
	int		quoted;

	if (psr_istok(TOK_WORD, tok) == false)
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

// 18/11 - Ok, clean out that mess...
// It's all about finding the EXPECTED tokens after these ones.
static int	psr_subtok_check(int *opn, t_tok *tok)
{
	if (tok->type != TOK_SUBO && tok->type != TOK_SUBC)
		return (0);

	if (tok->type == TOK_SUBO)
		(*opn)++;
	else if (tok->type == TOK_SUBC)
		(*opn)--;


	// if opn == 0, last is ')'
		// but


	// New system for printing ERR_SYNTAX with the right token?
	if ((tok->type == TOK_SUBC && *opn < 0 && psr_synterr(tok))
		|| (tok[1].type == TOK_EOL && *opn > 0 && psr_synterr(tok + 1)))
		return (-1);
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
		if (psr_redtok_check(tok) == -1
			|| psr_binaop_check(i, tok) == -1
			|| psr_wtok_check(tok) == -1
			|| psr_subtok_check(&opn, tok) == -1)
			return (core->exit = 2, -1);
		tok = toks + ++i;
	}
	return (0);
}
