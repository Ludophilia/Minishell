/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:02:46 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/16 20:22:40 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_redtok_check(t_tok *tok)
{
	if (tok->type < TOK_IRED || tok->type > TOK_ORED_AP)
		return (0);
	if (tok[1].type != TOK_WORD)
	{
		psr_synterr(tok + 1);
		return (-1);
	}
	return (0);
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

	// 16/11 - Correct cases...

	// (()) (not a syntax error)
	// ((())) (syntax error but another operator, so leave it)

	
	// 16/11 - Faulty cases...
	
	// 		()

	// 		( && )
	// 		( | )
	// 		( || )

	//		()()
	//		()()()

	// There must be something between pars
	//	- Another par
	//	- 


	
	return (pos == 0 || tok[1].type == TOK_EOL);
}
