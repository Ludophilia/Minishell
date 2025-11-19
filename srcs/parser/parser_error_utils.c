/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:02:46 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/20 00:12:38 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_isop(t_tok *tok)
{
	return (tok->type == TOK_PIPE
		|| tok->type == TOK_AND
		|| tok->type == TOK_OR);
}

int	psr_isired(t_tok *tok)
{
	return (tok->type == TOK_IRED
		|| tok->type == TOK_IRED_HD);
}

int	psr_isored(t_tok *tok)
{
	return (tok->type == TOK_ORED
		|| tok->type == TOK_ORED_AP);
}

int	psr_istok(t_tokt type, t_tok *tok)
{
	return (tok->type == type);
}

int	psr_synterr(t_tok *tok)
{
	char	str[128];

	if (tok->type != TOK_EOL)
		ft_strlcpy(str, tok->start, tok->len + 1);
	else
		ft_strlcpy(str, "end-of-line", 12);
	ft_eprintf(ERR_SYNTAX, str);
	return (1);
}
