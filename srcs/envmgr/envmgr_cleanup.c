/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envmgr_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 22:46:09 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/13 23:01:14 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// gestion des erreurs lors de la duplication de l'env
static t_env	*env_dup_fail(char *key, char *val, t_env *list)
{
	free(key);
	free(val);
	env_free(list);
	return (NULL);
}

// libère toute la liste chaînée
void	env_free(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

// 13/10 - Gere ce type de cas, individuellement...
/*		free(env->key);
		free(env->value);
		free(env); */ 
// void	env_del()
// {
	
// }
