/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 13:37:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/14 21:49:47 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_optok_check(t_tok *tok, int pos)
{
	char	**tok_chr;

	if (tok->type == TOK_WORD || tok->type == TOK_SUBO
		|| tok->type == TOK_SUBC)
		return (0);
	tok_chr = (char *[]){"|", "<", "<<", ">", ">>", "&&", "||", 0};
	if (tok[1].type == TOK_EOL
		|| psr_redtok_check(tok)
		|| psr_duplop_check(tok)
		|| psr_binaop_check(pos, tok))
	{
		ft_eprintf(ERR_SYNTAX, tok_chr[tok->type - 2]);
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

static int	psr_subtok_check(int *opn, t_tok *tok)
{
	if (tok->type != TOK_SUBO && tok->type != TOK_SUBC)
		return (0);
	if (tok->type == TOK_SUBO)
		(*opn)++;
	else if (tok->type == TOK_SUBC)
		(*opn)--;
	if ((tok->type == TOK_SUBC && *opn < 0 && ft_eprintf(ERR_SYNTAX, ")"))
		|| (tok[1].type == TOK_EOL && *opn > 0 && ft_eprintf(ERR_SYNTAX, "(")))
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
