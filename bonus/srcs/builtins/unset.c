/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 22:31:19 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/15 12:42:07 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction pour supprimer une variable d'environnement
static void	unset_arg(t_env **env, const char *key)
{
	t_env	*tmp;
	t_env	*prev;

	prev = NULL;
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key) + 1) == 0)
		{
			env_del_node(tmp, prev, env);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	bi_unset(t_cmd *cmd, t_env **env)
{
	int		i;
	char	*arg;

	i = 1;
	arg = cmd->argv[i];
	while (arg)
	{
		unset_arg(env, arg);
		arg = cmd->argv[i++];
	}
	return (0);
}
