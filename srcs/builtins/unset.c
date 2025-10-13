/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 22:31:19 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/13 23:09:33 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction pour supprimer une variable d'environnement
static void	env_unset(t_env **env, const char *key)
{
	t_env	*tmp;
	t_env	*prev;

	prev = NULL;
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key) + 1) == 0)
		{
			// 13/10 - There could be a logic that free that thing and
			// and restructure the nodes?
			// prev, node, **env
			
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
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
		env_unset(env, arg);
		arg = cmd->argv[i++];
	}
	return (0);
}

// int	bi_unset(t_core *core, t_cmd *cmd, t_env **env)
// {
// 	int		i;
// 	char	*arg;

// 	i = 1;
// 	arg = cmd->argv[i];
// 	while (arg)
// 	{
// 		if (!env_is_identifier(arg))
// 		{
// 			ft_eprintf(ERR_UINV, arg);
// 			core->exit = 1;
// 		}
// 		else
// 			env_unset(env, arg);
// 		arg = cmd->argv[i++];
// 	}
// 	return (0);
// }
