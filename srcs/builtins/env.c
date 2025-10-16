/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:06:19 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/13 22:26:03 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_env(t_env *env, int fd)
{
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->key, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(env->value, fd);
		}
		env = env->next;
	}
	return (0);
}
