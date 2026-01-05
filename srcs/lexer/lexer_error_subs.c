/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error_subs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 19:50:25 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/05 02:23:11 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lex_aftersub_check(int *pos, t_tok *tok)
{
	t_tok	*next;
	int		i;

	i = 0;
	next = tok + ++i;
	while (next->type != TOK_EOL && lex_isop(next) == false)
	{
		if (next->type == TOK_WORD || lex_issub(next))
		{
			lex_synterr(next->type, next);		
			return (-1);
		}
		if (lex_isred(next))
		{
			if ((next + 1)->type != TOK_WORD)
			{
				lex_synterr((next + 1)->type, next + 1);
				return (-1);
			}
			i++;
		}
		next = tok + ++i;
	}
	*pos += (i - 1);
	return (0);
}

int	lex_subtok_check(int *pos, int *opn, t_tok *tok, t_tok *next)
{
	if (tok->type != TOK_SUBO && tok->type != TOK_SUBC)
		return (0);
	if (tok->type == TOK_SUBO)
		(*opn)++;
	else
		(*opn)--;
	if (tok->type == TOK_SUBC && (*pos == 0 || *opn < 0))
	{
		lex_synterr(tok->type, tok);
		return (-1);
	}
	if (tok->type == TOK_SUBO && (lex_isop(next) || next->type == TOK_SUBC))
	{
		lex_synterr(next->type, next);
		return (-1);
	}
	if (tok->type == TOK_SUBC && lex_aftersub_check(pos, tok) == -1)
		return (-1);
	return (0);
}
