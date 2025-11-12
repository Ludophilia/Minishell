/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 22:46:09 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/18 17:04:39 by jegerman         ###   ########.fr       */
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

int	env_del_node(t_env *node, t_env *prev, t_env **env_list)
{
	if (prev)
		prev->next = node->next;
	else
		*env_list = node->next;
	env_free_node(node);
	return (1);
}

// libère toute la liste chaînée
int	env_free_all(t_env *node)
{
	t_env	*tmp;

	if (node == NULL)
		return (1);
	while (node)
	{
		tmp = node->next;
		env_free_node(node);
		node = tmp;
	}
	return (1);
}

int	env_cleanup(t_core *core)
{
	env_free_all(core->env);
	core->env = NULL;
	if (core->envp)
	{
		utl_free_strs(0, core->envp);
		core->envp = NULL;
	}
	return (1);
}
