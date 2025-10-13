/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envmgr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 20:31:40 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/13 23:00:31 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// crée un nouveau noeud d'environnement
t_env	*env_new(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env)); // 13/10 - THey almost do the same thing....
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

// ajoute un noeud à la fin de la liste chaînée
void	env_add(t_env **list, t_env *new)
{
	t_env	*last;

	if (!list || !new)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new;
}

// Fonction pour ajouter un nouveau nœud au début de la liste chaînée
void	env_add_node(t_env **env, const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env)); // 13/10 - THey almost do the same thing....
	if (!new)
		return ;
	new->key = ft_strdup(key);
	if (!new->key)
	{
		free(new);
		return ;
	}
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	if (value && !new->value)
	{
		free(new->key);
		free(new);
		return ;
	}
	new->next = *env;
	*env = new;
}

// duplique le tableau envp en une liste chaînée
t_env	*env_dup(char **envp)
{
	t_env	*env_list;
	t_env	*new;
	char	*sep;
	char	*key;
	char	*val;

	env_list = NULL;
	while (*envp)
	{
		sep = ft_strchr(*envp, '=');
		if (sep)
		{
			key = ft_substr(*envp, 0, sep - *envp);
			val = ft_strdup(sep + 1);
			if (!key || !val)
				return (env_dup_fail(key, val, env_list));
			new = env_new(key, val);
			if (!new)
				return (env_dup_fail(key, val, env_list));
			env_add(&env_list, new);
		}
		envp++;
	}
	return (env_list);
}
