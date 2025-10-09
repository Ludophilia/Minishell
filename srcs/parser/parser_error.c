/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 13:37:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/09 16:10:25 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_optok_check(t_tok *tok, int pos)
{
	char	**tok_chr;

	if (tok->type == TOK_WORD)
		return (0);
	tok_chr = (char *[]){"|", "<", "<<", ">", ">>", 0};
	if (tok[1].type == TOK_EOL
		|| tok[1].type == tok->type || (pos == 0 && tok->type == TOK_PIPE))
	{
		ft_eprintf(ERR_SYNTAX, tok_chr[tok->type - 2]);
		return (-1);
	}
	return (0);
}

static int	psr_wtok_check(t_tok *tok)
{
	int	i;
	int	quoted;

	if (tok->type != TOK_WORD)
		return (0);
	i = -1;
	quoted = 0;
	while (++i < tok->len)
	{
		if (!quoted && lex_is_quote(tok->start[i]))
			quoted = tok->start[i];
		else if (quoted && tok->start[i] == quoted)
			quoted = 0;
	}
	if (quoted != 0 && ft_eprintf(ERR_SYNTAX, "end-of-line"))
		return (-1);
	return (0);
}

int	psr_error_check(t_tok *toks)
{
	t_tok	*tok;
	int		i;

	i = 0;
	tok = toks + i;
	while (tok->type != TOK_EOL)
	{
		if (psr_optok_check(tok, i) == -1 || psr_wtok_check(tok) == -1)
			return (-1);
		tok = toks + ++i;
	}
	return (0);
}
