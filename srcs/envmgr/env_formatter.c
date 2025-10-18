/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_formatter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:10:32 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/18 20:55:17 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_get_size(t_env *env_list)
{
	int		size;
	t_env	*env;

	size = 0;
	env = env_list;
	while (env)
	{
		if (env->value)
			size++;
		env = env->next;
	}
	return (size);
}

static char	*env_to_str(char *key, char *val)
{
	char	*str;
	int		size;
	int		i;

	if (!key || !val)
		return (NULL);
	size = ft_strlen(key) + 1 + ft_strlen(val);
	str = ft_calloc(size + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (*key)
		str[i++] = *key++;
	str[i++] = '=';
	while (*val)
		str[i++] = *val++;
	return (str);
}

char	**env_get_envp(t_env *env_list)
{
	t_env	*env;
	int		size;
	char	**envp;
	int		j;

	size = env_get_size(env_list);
	envp = ft_calloc(size + 1, sizeof(char *));
	if (envp == NULL)
		return (NULL);
	env = env_list;
	j = 0;
	while (env)
	{
		if (env->value)
		{
			envp[j] = env_to_str(env->key, env->value);
			if (!envp[j])
				return (utl_free_strs(0, envp), NULL);
			j++;
		}
		env = env->next;
	}
	return (envp);
}

