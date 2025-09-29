/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:18:23 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/29 19:33:48 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	utl_cleanup(t_cflg flags, t_core *core)
{
	if (flags & FLG_CMDS)
		psr_cleanup_cmds(core);
	return (1);
}

char	*utl_shitoa(unsigned int nbr, char *store)
{
	if (nbr >= 10)
		store = utl_shitoa(nbr / 10, store);
	*store++ = '0' + (nbr % 10);
	*store = 0;
	return (store);
}
