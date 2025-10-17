/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:06:11 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/17 22:24:22 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Affichage de export sans arguments
static void	print_export(t_env *env_list)
{
	t_env	*env;

	env = env_list;
	while (env)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->key, STDOUT_FILENO);
		if (env->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
		env = env->next;
	}
}

// gère chaque argument de export
static int	handle_export_arg(t_env **env_list, char *arg)
{
	char	*eq;

	if (!env_is_identifier(arg))
	{
		ft_eprintf(ERR_BINV, "export", arg);
		return (-1);
	}
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*eq = '\0';
		if (env_set(env_list, arg, eq + 1) == -1)
		{
			*eq = '=';
			return (-1);
		}
		*eq = '=';
	}
	else
	{
		if (!env_get(*env_list, arg))
			env_set(env_list, arg, NULL);
	}
	return (0);
}

int	bi_export(t_cmd *cmd, t_env **env_list)
{
	int	err_count;
	int	i;

	if (!cmd || !env_list)
		return (1);
	if (cmd->argc == 1)
	{
		print_export(*env_list);
		return (0);
	}
	i = 1;
	err_count = 0;
	while (cmd->argv[i])
	{
		if (handle_export_arg(env_list, cmd->argv[i]) == -1)
			err_count++;
		i++;
	}
	return (err_count && 1);
}
