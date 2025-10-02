/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:18:23 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/02 00:40:46 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	utl_cleanup(t_cflg flags, t_core *core)
{
	// printf("[%i] cleaning...\n", getpid());
	// printf("\t[%i] fcmds -> %i\n", getpid(), (flags & FLG_CMDS));
	// printf("\t[%i] freds -> %i\n", getpid(), (flags & FLG_REDS));
	if ((flags & FLG_CMDS) || (flags & FLG_REDS))
		psr_cleanup_cmds(flags, core);
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
