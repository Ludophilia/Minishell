/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:02:46 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/14 21:58:15 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_redtok_check(t_tok *tok)
{
	if (tok->type < TOK_IRED || tok->type > TOK_ORED_AP)
		return (0);
	return (tok[1].type != TOK_WORD);
}

int	psr_duplop_check(t_tok *tok)
{
	return (tok[1].type >= TOK_PIPE && tok[1].type <= TOK_OR);
}

int	psr_binaop_check(int pos, t_tok *tok)
{
	t_tokt	type;

	type = tok->type;
	if (type != TOK_PIPE && type != TOK_AND && type != TOK_OR)
		return (0);
	return (pos == 0 || tok[1].type == TOK_EOL);
}
