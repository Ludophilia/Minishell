/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2024/03/13 19:52:01 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint16_t	g_exit_status = 0;

int	main(void)
{
	t_core	core;

	// (&core)->exit = &g_exit_status;
	if (sigmgr_init_handlers() == -1
		|| intf_loop_prompt(&core) == -1)
		return (1);
	return (0);
}
