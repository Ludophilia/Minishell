/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:26:53 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/16 19:09:53 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exc_exec_builtin(int id, t_cmd *cmd, t_core *core)
{
	int	status;

	if (!cmd->argv || !cmd->argv[0])
		return (1);
	status = 1;
	if (id == 0)
		status = bi_echo(cmd, cmd->ofd);
	else if (id == 1)
		status = bi_cd(core, cmd);
	else if (id == 2)
		status = bi_pwd(cmd->ofd);
	else if (id == 3)
		status = bi_export(cmd, &core->env);
	else if (id == 4)
		status = bi_unset(cmd, &core->env);
	else if (id == 5)
		status = bi_env(core->env, cmd->ofd);
	else if (id == 6)
		status = bi_exit(core, cmd);
	return (status);
}

int	exc_if_builtin(t_cmd *cmd, t_core *core)
{
	char	**bltns;
	int		i;
	
	if (*cmd->argv == NULL)
		return (-1);
	bltns = (char *[]){"echo", "cd", "pwd", "export", "unset", "env",
		"exit", 0};
	i = 0;
	while (bltns[i])
	{
		if (!ft_strncmp(*cmd->argv, bltns[i], ft_strlen(bltns[i]) + 1))
		{
			core->exit = exc_exec_builtin(i, cmd, core);
			return (1);
		}
		i++;
	}
	return (0);
}
