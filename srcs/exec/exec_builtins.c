/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:26:53 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/12 21:43:43 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 3/10 - Change la logique si tu veux tant que ca fonctionne...
// Il faut que int	exc_exec_cmd(t_cmd *cmd, t_core *core)
// Fasse la difference entre une commande standard et un builtin
// Tout est execute dans enfant, meme les builtins...
// int	exc_is_builtin(char *arg)
// {
// 	char	**builtins;
// 	int		j;

// 	builtins = (char *[8]){"echo", "cd", "pwd", "export",
// 		"unset", "env", "exit", 0};
// 	j = -1;
// 	while (builtins[++j])
// 		if (!ft_strncmp(arg, builtins[j], ft_strlen(builtins[j])))
// 			return (j);
// 	return (0);
// }

// 10/10 - Real work begins here
int	exc_exec_builtin(t_core *core, t_cmd *cmd, int fd)
{
	int	id;
	int	status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	id = exc_is_builtin(cmd->argv[0]);
	status = 0;
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
	else
		status = 1;
	g_exit_status = status;
	return (status);
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
