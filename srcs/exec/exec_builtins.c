/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:26:53 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/08 23:27:12 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 3/10 - Change la logique si tu veux tant que ca fonctionne...
// Il faut que int	exc_exec_cmd(t_cmd *cmd, t_core *core)
// Fasse la difference entre une commande standard et un builtin
// Tout est execute dans enfant, meme les builtins...
/*int	exc_is_builtin(char *arg)
{
	char	**builtins;
	int		j;

	builtins = (char *[8]){"echo", "cd", "pwd", "export",
		"unset", "env", "exit", 0};
	j = -1;
	while (builtins[++j])
		if (!ft_strncmp(arg, builtins[j], ft_strlen(builtins[j])))
			return (j);
	return (0);
}*/

int	exc_exec_builtin(t_core *core, t_cmd *cmd, int fd)
{
	int	i;

	i = exc_is_builtin(cmd->argv[0]);
	if (i == 0)
		return (bi_echo(cmd, fd));
	if (i == 1)
		return (bi_cd(core, cmd));
	if (i == 2)
		return (bi_pwd(fd));
	if (i == 3)
		return (bi_export(cmd, &core->env));
	if (i == 4)
		return (bi_unset(cmd, &core->env));
	if (i == 5)
		return (bi_env(core->env, fd));
	if (i == 6)
		return (bi_exit(core, cmd));
	return (1);
}


int	exc_is_builtin(char *arg)
{
	static const char	*builtins[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
	};
	int	i;

	if (!arg)
		return (-1);
	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(arg, builtins[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}


// int	exc_builtins(int id, char **argv, char **envp)
// {
// 	int (**arr)();

// 	arr = (int (*[8])()){, NULL};

// 	arr[id]();
// }
