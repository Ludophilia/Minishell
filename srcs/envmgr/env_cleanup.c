/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 22:46:09 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/14 22:13:36 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_free_node(t_env *node)
{
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	if (node)
		free(node);
	return (1);
}

int	env_del_node(t_env *node, t_env *prev, t_env **head)
{
	if (prev)
		prev->next = node->next;
	else
		*head = node->next;
	env_free_node(node);
	return (1);
}

// libère toute la liste chaînée
int	env_free_all(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		env_free_node(env);
		env = tmp;
	}
	return (1);
}
