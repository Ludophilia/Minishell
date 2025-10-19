/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 20:07:06 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/19 01:46:22 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_cmds(t_cmd *cmds)
{
	int	i;

	i = -1;
	while (++i < CMD_MAX)
	{
		ft_bzero(cmds[i].reds, RED_MAX * sizeof(t_red));
		cmds[i].xready = 0;
		cmds[i].pid = 0;
		cmds[i].argv = 0;
		cmds[i].argc = 0;
		cmds[i].ifd = 0;
		cmds[i].ofd = 1;
	}
}

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
	init_cmds(core->cmds);
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
