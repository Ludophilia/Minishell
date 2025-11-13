/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 13:37:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/13 21:10:18 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_redtok_check(t_tok *tok)
{
	if (tok->type < TOK_IRED || tok->type > TOK_ORED_AP)
		return (0);
	return (tok[1].type != TOK_WORD);
}

static int	psr_duplop_check(t_tok *tok)
{
	return (tok[1].type >= TOK_PIPE && tok[1].type <= TOK_OR);
}

static int	psr_binaop_check(int pos, t_tok *tok)
{
	t_tokt	type;

	type = tok->type;
	if (type != TOK_PIPE && type != TOK_AND && type != TOK_OR)
		return (0);
	return (pos == 0 || tok[1].type == TOK_EOL);
}

static int	psr_optok_check(t_tok *tok, int pos, t_core *core)
{
	char	**tok_chr;

	if (tok->type == TOK_WORD || tok->type == TOK_SUBO
		|| tok->type == TOK_SUBC)
		return (0);
	tok_chr = (char *[]){"|", "<", "<<", ">", ">>",
		"&&", "||", "(", ")", 0};
	if (tok[1].type == TOK_EOL
		|| psr_redtok_check(tok)
		|| psr_duplop_check(tok)
		|| psr_binaop_check(pos, tok))
	{
		ft_eprintf(ERR_SYNTAX, tok_chr[tok->type - 2]);
		core->exit = 2;
		return (-1);
	}
	return (0);
}

static int	psr_wtok_check(t_tok *tok, t_core *core)
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
	{
		core->exit = 2;
		return (-1);
	}
	return (0);
}


// 13/11 - Adding && () || management.
// A function to see if parentheses are matched (before the main loop)


// 13/11 - Alright, here we are.
int	psr_error_check(t_tok *toks, t_core *core)
{
	t_tok	*tok;
	int		i;

	i = 0;
	tok = toks + i;
	while (tok->type != TOK_EOL)
	{
		if (psr_optok_check(tok, i, core) == -1
			|| psr_wtok_check(tok, core) == -1)
			return (-1);
		tok = toks + ++i;
	}
	return (0);
}
