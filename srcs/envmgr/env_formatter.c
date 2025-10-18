/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_formatter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:10:32 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/18 17:41:29 by jegerman         ###   ########.fr       */
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
		env = (size++, env->next);
	return (size);
}

static char	*env_to_str(char *key, char *val, char **envp, int j)
{
	char	*str;
	int		size;
	int		i;

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
	envp[j] = str;
	return (str);
}

char	**env_get_envp(t_env *env_list, t_core *core)
{
	char	**envp;
	t_env	*env;
	int		size;
	int		j;

	if (core->envp && utl_free_strs(0, core->envp))
		core->envp = NULL;
	size = env_get_size(env_list);
	envp = ft_calloc(size + 1, sizeof(char *));
	if (envp == NULL)
		return (NULL);
	env = env_list;
	j = 0;
	while (env)
	{
		if (!env_to_str(env->key, env->value, envp, j++))
		{
			utl_free_strs(0, envp);
			return (NULL);
		}
		env = env->next;
	}
	return (core->envp = envp);
}
