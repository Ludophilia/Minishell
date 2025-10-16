/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 20:31:40 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/16 19:12:43 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// crée un nouveau noeud d'environnement
t_env	*env_new_node(const char *key, const char *value)
{
	t_env	*new;

	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (!new->key)
	{
		env_free_node(new);
		return (NULL);
	}
	if (value)
		new->value = ft_strdup(value);
	if (value && !new->value)
	{
		env_free_node(new);
		return (NULL);
	}
	return (new);
}

// ajoute un noeud à la fin de la liste chaînée
int	env_add_end(t_env **env_list, const char *key, const char *value)
{
	t_env	*new;
	t_env	*last;

	if (!env_list)
		return (-1);
	new = env_new_node(key, value);
	if (!new)
		return (-1);
	if (!*env_list)
	{
		*env_list = new;
		return (0);
	}
	last = *env_list;
	while (last->next)
		last = last->next;
	last->next = new;
	return (0);
}

// Fonction pour ajouter un nouveau nœud au début de la liste chaînée
int	env_add_start(t_env **env_list, const char *key, const char *value)
{
	t_env	*new;

	if (!env_list)
		return (-1);
	new = env_new_node(key, value);
	if (!new)
		return (-1);
	if (*env_list)
		new->next = *env_list;
	*env_list = new;
	return (0);
}

// duplique le tableau envp en une liste chaînée
t_env	*env_dup(char **envp)
{
	t_env	*env_list;
	char	*sep;

	env_list = NULL;
	while (*envp)
	{
		sep = ft_strchr(*envp, '=');
		if (sep)
		{
			*sep = 0;
			if (env_add_end(&env_list, *envp, sep + 1) == -1)
			{
				env_free_all(env_list);
				return (NULL);
			}
		}
		envp++;
	}
	return (env_list);
}
