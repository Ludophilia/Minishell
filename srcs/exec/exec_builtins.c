/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:26:53 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/07 17:10:41 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 3/10 - Change la logique si tu veux tant que ca fonctionne...
// Il faut que int	exc_exec_cmd(t_cmd *cmd, t_core *core)
// Fasse la difference entre une commande standard et un builtin
// Tout est execute dans enfant, meme les builtins...
int	exc_is_builtin(char *arg)
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
}
