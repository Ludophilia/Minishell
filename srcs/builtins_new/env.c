/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 20:31:40 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/09 16:01:13 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction pour obtenir la valeur d'une variable d'environnement
char	*get_env(t_env *env, const char *key)
{
	if (!env || !key)
		return (NULL);
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

// Fonction pour ajouter un nouveau nœud au début de la liste chaînée
void	add_env_node(t_env **env, const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
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

// Fonction pour définir ou mettre à jour une variable d'environnement
void	set_env(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;

	if (!env || !key)
		return ;
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key) + 1) == 0)
		{
			free(tmp->value);
			if (value)
				tmp->value = ft_strdup(value);
			else
				tmp->value = NULL;
			return ;
		}
		tmp = tmp->next;
	}
	add_env_node(env, key, value);
}

// Fonction pour supprimer une variable d'environnement
void	unset_env(t_env **env, const char *key)
{
	t_env	*tmp;
	t_env	*prev;

	prev = NULL;
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key) + 1) == 0)
		{
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
		if (!is_valid_identifier(arg))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			g_exit_status = 1;
		}
		else
			unset_env(env, arg);
		i++;
		arg = cmd->argv[i];
	}
	return (0);
}
