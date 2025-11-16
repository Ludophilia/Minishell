/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 13:37:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/16 20:29:43 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_synterr(t_tok *tok)
{
	char		str[128];
	int			i;

	if (tok->type != TOK_EOL)
	{
		i = -1;
		while (++i < tok->len)
			str[i] = tok->start[i];
		str[i] = '\0';
	}
	else
		ft_strlcpy(str, "end-of-line", 12);
	return (ft_eprintf(ERR_SYNTAX, str));
}

static int	psr_optok_check(t_tok *tok, int pos)
{
	char	**tok_chr;

	if (tok->type == TOK_WORD || tok->type == TOK_SUBO
		|| tok->type == TOK_SUBC)
		return (0);
	tok_chr = (char *[]){"|", "<", "<<", ">", ">>", "&&", "||", 0};
	if (tok[1].type == TOK_EOL
		|| psr_redtok_check(tok) == -1
		|| psr_duplop_check(tok)
		|| psr_binaop_check(pos, tok))
	{
		// printf("fake"); psr_synterr(tok); // Yeah... Remove it then.
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
	if (quoted != 0 && ft_eprintf(ERR_SYNTAX, "end-of-line"))
		return (-1);
	return (0);
}

// 16/11 - Yeah, there's A LOT left to be done here.
//	- Improve syntax error management. It's all about expectations.
//	- Syntax: how are things put together, which things, in which order at least
//	here.
//	- Syntax error: A TOKEN SHOULDN'T BE HERE.

// 16/11
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
		if (psr_optok_check(tok, i) == -1
			|| psr_wtok_check(tok) == -1
			|| psr_subtok_check(&opn, tok) == -1)
			return (core->exit = 2, -1);
		tok = toks + ++i;
	}
	return (0);
}
