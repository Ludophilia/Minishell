/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:24:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/09 20:20:52 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exp_is_outq(int c, int *quo)
{
	if ((*quo == 0 && lex_is_quote((char *)&c))
		|| (*quo != 0 && *quo == c))
	{
		if (*quo == 0)
			*quo = c;
		else
			*quo = 0;
		return (1);
	}
	return (0);
}

int	exp_is_envv_chr(int c, int pos)
{
	if ((pos == 0 && (ft_isalpha(c) || c == '_'))
		|| (ft_isalnum(c) || c == '_'))
		return (1);
	return (0);
}

int	exp_is_envv(char *c, int token, int quote)
{
	if (*c == '$'
		&& token != TOK_IRED_HD
		&& quote != '\''
		&& (c[1] == '?'
			|| exp_is_envv_chr(c[1], 0)))
		return (1);
	return (0);
}
