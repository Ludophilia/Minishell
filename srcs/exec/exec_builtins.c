/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:26:53 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/13 16:53:10 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exc_exec_builtin(t_core *core, t_cmd *cmd, int fd)
{
	int	id;
	int	status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	id = exc_is_builtin(cmd->argv[0]);
	status = 1;
	if (id == 0)
		status = bi_echo(cmd, fd);
	else if (id == 1)
		status = bi_cd(core, cmd);
	else if (id == 2)
		status = bi_pwd(fd);
	else if (id == 3)
		status = bi_export(cmd, &core->env);
	else if (id == 4)
		status = bi_unset(cmd, &core->env);
	else if (id == 5)
		status = bi_env(core->env, fd);
	else if (id == 6)
		status = bi_exit(core, cmd);
	return (core->exit = status);
}

int	exc_is_builtin(char *arg)
{
	static const char	*builtins[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
	int					i;

	if (!arg)
		return (-1);
	i = 0;
	while (builtins[i])
	{
		if (ft_strncmp(arg, builtins[i], ft_strlen(builtins[i]) + 1) == 0)
			return (i);
		i++;
	}
	return (-1);
}
