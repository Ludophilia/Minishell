/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/16 19:21:11 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint32_t	g_sig = 0;

static int	main_init_core(t_core *core, char **envp)
{
	core->flags = 0;
	core->cmd_pmax = 0;
	core->cmd_xrdy = 0;
	core->env = env_dup(envp);
	if (core->env == NULL)
		return (-1);
	core->exit = 0;
	ft_bzero(core->cmds, CMD_MAX * sizeof(t_cmd));
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_core	core;

	if ((++argv, --argc != 0) && ft_eprintf(ERR_USAGE))
		return (1);
	if (main_init_core(&core, envp) == -1)
		return (2);
	if (loop_prompt(&core) == -1 && utl_cleanup(FLG_ENV, &core))
		return (3);
	return (core.exit);
}
