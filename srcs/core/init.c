/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 20:07:06 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/09 16:14:54 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_core(t_core *core, char **envp)
{
	core->flags = 0;
	core->cmd_pmax = 0;
	core->cmd_xrdy = 0;
	core->env = env_dup(envp);
	if (core->env == NULL)
		return (-1);
	core->envp = NULL;
	core->exit = 0;
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

int	init_cleanup_core(t_core *core)
{
	core->cmd_pmax = 0;
	core->cmd_xrdy = 0;
	return (1);
}
