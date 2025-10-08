/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/06 00:52:24 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint32_t	g_exit_status = 0; // signals stops me from removing it...

int	main(int argc, char **argv, char **envp)
{
	t_core	core;

	if (++argv, --argc != 0)
		return (1);
	core.envp = envp; // provisory
	// (&core)->exit = &g_exit_status;
	if (sig_init_handlers() == -1
		|| ui_loop_prompt(&core) == -1)
		return (2);
	return (0);
}
