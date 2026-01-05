/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:02:46 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/05 02:23:24 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lex_isop(t_tok *tok)
{
	return (tok->type == TOK_PIPE
		|| tok->type == TOK_AND
		|| tok->type == TOK_OR);
}

int	lex_isred(t_tok *tok)
{
	return (tok->type == TOK_IRED
		|| tok->type == TOK_IRED_HD
		|| tok->type == TOK_ORED
		|| tok->type == TOK_ORED_AP);
}

int	lex_issub(t_tok *tok)
{
	return (tok->type == TOK_SUBO
		|| tok->type == TOK_SUBC);
}

int	lex_istok(t_tokt type, t_tok *tok)
{
	return (tok->type == type);
}

int	lex_synterr(t_tokt type, t_tok *tok)
{
	char	str[128];

	if (type == TOK_EOL)
		ft_strlcpy(str, "end-of-line", 12);
	else
		ft_strlcpy(str, tok->start, tok->len + 1);
	ft_eprintf(ERR_SYNTAX, str);
	return (1);
}
