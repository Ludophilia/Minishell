/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 20:29:58 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/16 19:13:09 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// vérifie si l'identifiant est valide
int	env_is_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Fonction pour obtenir la valeur d'une variable d'environnement
char	*env_get(t_env *env_list, const char *key)
{
	t_env	*env;

	if (!env_list || !key)
		return (NULL);
	env = env_list;
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static int	env_upd_value(t_env *env, const char *value)
{
	if (value && env->value)
	{
		free(env->value);
		env->value = NULL;
	}
	if (value)
	{
		env->value = ft_strdup(value);
		if (!env->value)
			return (-1);
	}
	return (0);
}

// Fonction pour définir ou mettre à jour une variable d'environnement
int	env_set(t_env **env_list, const char *key, const char *value)
{
	t_env	*env;

	if (!env_list || !key)
		return (-1);
	env = *env_list;
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env_upd_value(env, value));
		env = env->next;
	}
	if (env_add_start(env_list, key, value) == -1)
		return (-1);
	return (0);
}
