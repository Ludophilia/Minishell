/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2025/09/05 18:20:39 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint32_t	g_exit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	t_core	core;

	if (++argv, --argc != 0)
		return (1);
	(void)envp; // 5/09 - Yeah, the environment, what to do with it?

	// (&core)->exit = &g_exit_status;
	if (sig_init_handlers() == -1
		|| ui_loop_prompt(&core) == -1)
		return (2);
	return (0);
}
