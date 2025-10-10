/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/10 21:59:57 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint32_t	g_exit_status = 0; // signals stops me from removing it...

// 10/10 - Maybe I should leave it in interface?
int	ui_init_core(t_core *core, char **envp)
{
	core->flags = 0;
	core->cmd_pmax = 0;
	core->cmd_xrdy = 0;
	core->envp = envp; // provisory
	// core->env = dup_env(envp);
	core->exitv = 0; // ??? Unsure about this one.
	ft_bzero(core->cmds, CMD_MAX * sizeof(t_cmd));
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_core	core;

	if (++argv, --argc != 0)
		return (1);
	// sig_init_handlers() == -1
	sig_init_prompt(); // 10/10 - Can that function even fail? we manage errors
	if (ui_loop_prompt(&core) == -1)
	{
		free_env(core.env);		
		return (2);
	}
	free_env(core.env);
	return (0);
}
