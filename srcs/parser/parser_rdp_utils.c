/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rdp_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:53:51 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/16 19:22:01 by jegerman         ###   ########.fr       */
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