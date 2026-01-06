/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 20:07:06 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/06 23:29:50 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_core(t_core *core, char **envp)
{
	core->flags = 0;
	core->env = env_dup(envp);
	if (core->env == NULL)
		return (-1);
	core->envp = NULL;
	core->ast = NULL;
	core->exit = 0;
	core->cmds = 0;
	ft_bzero(core->stash, NOD_MAX * sizeof(t_astn *));
	core->flags |= FLG_ENV;
	return (0);
}

int	init_isatty(void)
{
	int	is_atty;

	is_atty = (isatty(STDIN_FILENO)
			&& isatty(STDOUT_FILENO)
			&& isatty(STDERR_FILENO));
	return (is_atty);
}
