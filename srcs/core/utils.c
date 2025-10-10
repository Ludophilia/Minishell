/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:18:23 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/10 21:45:36 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	utl_cleanup(t_cflg flags, t_core *core)
{
	// 10/10 - We will have to merge the two cleanup system... 
	if ((flags & FLG_CMDS) || (flags & FLG_REDS))
		psr_cleanup_cmds(flags, core);
	return (1);
}

int	utl_free_strs(int from_id, char **strs)
{
	int	j;

	j = 0;
	while (strs[from_id + j])
		free(strs[from_id + j++]);
	free(strs);
	return (1);
}

int	utl_free(void *ptr)
{
	free(ptr);
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
