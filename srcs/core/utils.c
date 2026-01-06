/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:18:23 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/06 23:38:53 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	utl_cleanup(t_cflg base_flags, t_cflg excl_flags, t_core *core)
{
	t_cflg	flags;

	flags = (base_flags & ~excl_flags);
	if (flags & FLG_AST)
	{
		psr_cleanup_ast(core->ast);
		ft_bzero(core->stash, NOD_MAX * sizeof(t_astn *));
		core->ast = NULL;
		core->cmds = 0;
	}
	if (flags & FLG_ENV)
		env_cleanup(core);
	if (flags & FLG_AST)
		core->flags &= ~FLG_AST;
	if (flags & FLG_ENV)
		core->flags &= ~FLG_ENV;
	return (1);
}

int	utl_exit(int status, t_core *core)
{
	utl_cleanup(core->flags, 0, core);
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
