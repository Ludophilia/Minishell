/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_envv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:09:10 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/09 20:45:22 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exp_get_envv_id(char *start, int *envv_len, char *envv_id)
{
	int	id_len;

	id_len = -1;
	if (*start == '?' && id_len++)
		envv_id[id_len++] = *start;
	else
		while (++id_len, exp_is_envv_chr(start[id_len], id_len))
			envv_id[id_len] = start[id_len];
	envv_id[id_len] = '\0';
	if (envv_len)
		*envv_len = (id_len + 1);
	return (0);
}

static char	*exp_get_envv_value(char *start, int *envv_len, t_core *core)
{
	char		envv_id[ID_LMAX];
	static char	exitw[EXW_MAX];
	char		*envv_val;

	exp_get_envv_id(start, envv_len, envv_id);
	if (!ft_strncmp(envv_id, "?", 2))
	{
		utl_itoa(core->exit, exitw);
		envv_val = ft_strdup(exitw);
	}
	else
	{
		envv_val = env_get(core->env, envv_id);
		if (envv_val)
			envv_val = ft_strdup(envv_val);
	}
	return (envv_val);
}

int	exp_envv_value_len(char *start, int *j, t_core *core)
{
	char	*envv_val;
	int		envv_len;
	int		len;

	envv_val = exp_get_envv_value(start + 1, &envv_len, core);
	if (envv_val == NULL)
		len = 0;
	else
		len = ft_strlen(envv_val);
	*j += envv_len;
	if (envv_val)
		free(envv_val);
	return (len);
}

int	exp_copy_envv_value(char *start, char *word, int *j, t_core *core)
{
	char	*envv_val;
	int		envv_len;
	int		pos;
	int		i;

	// 9/01 = Something won't work here... with subshells.
	envv_val = exp_get_envv_value(start + 1, &envv_len, core);
	if (envv_val == NULL)
		return (envv_len);
	pos = *j;
	i = 0;
	while (envv_val[i])
		word[pos++] = envv_val[i++];
	*j = pos;
	free(envv_val);
	return (envv_len);
}
