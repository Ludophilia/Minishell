/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/29 15:31:51 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint16_t	g_exit_status = 0;

int	main(void)
{
	t_core	core;

	(&core)->exit = &g_exit_status;
	if (sig_init_handlers() == -1
		|| intf_loop_prompt(&core) == -1)
		return (1);
	return (0);
}
