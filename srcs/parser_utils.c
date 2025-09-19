/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:24:36 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/20 00:00:39 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_is_ired(t_tok *tok)
{
	return (tok->type == TOK_IRED || tok->type == TOK_IRED_HD);
}

int	psr_is_ored(t_tok *tok)
{
	return (tok->type == TOK_ORED || tok->type == TOK_ORED_AP);
}

int	psr_is_outq(int c, int *q)
{
	if ((*q == 0 && lex_is_quote(c)) || (*q != 0 && *q == c))
		return ((*q == 0 && (*q = c)) || !(*q = 0));
	return (0);
}

int	psr_is_envv_chr(int c, int pos)
{
	if (pos == 0)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}


int	psr_is_envv(char *c, int ct, int q)
{
	if (*c == '$' && ct != TOK_IRED_HD && q != '\''
		&& (c[1] == '?' || psr_is_envv_chr(c[1], 0)))
		return (1);
	return (0);
}
