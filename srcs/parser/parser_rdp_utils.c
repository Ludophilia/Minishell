/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rdp_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:53:51 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/13 17:56:43 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdn	*psr_new_cmdn(t_astt type)
{
	t_cmdn	*new;

	new = ft_calloc(1, sizeof(t_cmdn));
	if (new == NULL)
		return (NULL);
	new->type = type;
	return (new);
}

t_astn	*psr_new_astn(t_astt type)
{
	t_astn	*new;

	new = ft_calloc(1, sizeof(t_astn));
	if (new == NULL)
		return (NULL);
	new->type = type;
	return (new);
}