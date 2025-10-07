/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:09:10 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/07 19:18:27 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_get_envv_id(char *start, int *envv_len, char *envv_id)
{
	int	id_len;

	id_len = -1;
	if (*start == '?' && id_len++)
		envv_id[id_len++] = *start;
	else
		while (++id_len, psr_is_envv_chr(start[id_len], id_len))
			envv_id[id_len] = start[id_len];
	envv_id[id_len] = '\0';
	if (envv_len)
		*envv_len = id_len + 1;
	return (0);
}

static char	*psr_get_envv_value(char *start, int *envv_len)
{
	static char	exitw[EXW_MAX];
	char		envv_id[ID_LMAX];
	char		*envv_val;

	psr_get_envv_id(start, envv_len, envv_id);
	if (ft_strncmp(envv_id, "?", 2) == 0 && utl_shitoa(g_exit_status, exitw)) // 7/10 - No longer g_exit_status
		envv_val = exitw;
	else
		envv_val = getenv(envv_id); // 7/10 - Stick to that env call?
	return (envv_val);
}

int	psr_envv_value_len(char *start, int *j)
{
	char	*envv_val;
	int		envv_len;
	int		len;

	envv_val = psr_get_envv_value(start + 1, &envv_len);
	if (envv_val == NULL)
		len = 0;
	else
		len = ft_strlen(envv_val);
	*j += envv_len;
	return (len);
}

int	psr_copy_envv_value(char *start, char *word, int *j)
{
	char	*envv_val;
	int		envv_len;
	int		pos;

	envv_val = psr_get_envv_value(start + 1, &envv_len);
	if (envv_val == NULL)
		return (envv_len);
	pos = *j;
	while (*envv_val)
		word[pos++] = *envv_val++;
	*j = pos;
	return (envv_len);
}
