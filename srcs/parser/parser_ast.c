/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:32:34 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/18 19:33:14 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_astn	*psr_new_astn(t_astt type)
{
	t_astn	*new;

	new = ft_calloc(1, sizeof(t_astn));
	if (new == NULL)
		return (NULL);
	new->type = type;
	return (new);
}

int	psr_build_ast(t_tok *toks, t_core *core)
{
	t_cnt	c;

	c.f = 0;
	c.i = 0;
	core->ast = psr_rdp_line(&c, toks, core);
	if (core->ast == NULL || (c.f > 0 && psr_cleanup_ast(core->ast)))
		return (-1);
	return (0);
}
