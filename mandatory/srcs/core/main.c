/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/19 01:52:21 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

int	main(int argc, char **argv, char **envp)
{
	t_core	core;

	if (init_isatty() != 1 || (++argv, --argc != 0))
	{
		ft_eprintf(ERR_USAGE);
		return (1);
	}
	if (init_core(&core, envp) == -1)
		return (2);
	if (loop_prompt(&core) == -1 && utl_cleanup(FLG_ENV, &core))
		return (3);
	return (core.exit);
}
