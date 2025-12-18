/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:18:23 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/18 18:54:08 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	utl_cleanup(t_cflg flags, t_core *core)
{
	// 18/12 - Completely broken
	// if ((flags & FLG_CMDS) || (flags & FLG_REDS))
	// 	psr_cleanup_cmds(flags, core);
	if (flags & FLG_AST)
		//
	if (flags & FLG_CORE)
		init_cleanup_core(core);
	if (flags & FLG_ENV)
		env_cleanup(core);
	if (flags & FLG_CMDS)
		core->flags &= ~FLG_CMDS;
	if (flags & FLG_REDS)
		core->flags &= ~FLG_REDS;
	if (flags & FLG_CORE)
		core->flags &= ~FLG_CORE;
	return (1);
}

int	utl_exit(int status, t_core *core)
{
	utl_cleanup(core->flags | FLG_ENV, core);
	core->exit = status;
	exit(status);
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

char	*utl_itoa(unsigned int nbr, char *store)
{
	if (nbr >= 10)
		store = utl_itoa(nbr / 10, store);
	*store++ = '0' + (nbr % 10);
	*store = 0;
	return (store);
}
