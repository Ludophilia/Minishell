/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:02:46 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/06 20:13:32 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_isop(t_tok *tok)
{
	return (tok->type == TOK_PIPE
		|| tok->type == TOK_AND
		|| tok->type == TOK_OR);
}

int	psr_isred(t_tok *tok)
{
	return (tok->type == TOK_IRED
		|| tok->type == TOK_IRED_HD
		|| tok->type == TOK_ORED
		|| tok->type == TOK_ORED_AP);
}

int	psr_istok(t_tokt type, t_tok *tok)
{
	return (tok->type == type);
}

int	psr_synterr(t_tokt type, t_tok *tok)
{
	char	str[128];

	if (type == TOK_EOL)
		ft_strlcpy(str, "end-of-line", 12);
	else
		ft_strlcpy(str, tok->start, tok->len + 1);
	ft_eprintf(ERR_SYNTAX, str);
	return (1);
}
