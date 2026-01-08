/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 23:04:50 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/08 15:03:41 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exc_load_envpath(char **envp, char ***paths)
{
	int		matched;
	int		i;

	*paths = NULL;
	matched = 0;
	i = -1;
	while (envp[++i])
		if (ft_strnstr(envp[i], "PATH", 4) && ++matched)
			break ;
	if (matched)
	{
		*paths = ft_split(envp[i] + 5, ':');
		if (*paths == NULL)
			return (-1);
	}
	return (0);
}

char	*exc_build_abspath(char **strs)
{
	char	*path;
	size_t	path_len;
	int		i;

	i = 0;
	path_len = 0;
	while (strs[i])
		path_len += ft_strlen(strs[i++]);
	path = ft_calloc(path_len + 1, sizeof(char));
	if (path == NULL)
		return (NULL);
	i = -1;
	while (strs[++i])
	{
		path_len = ft_strlen(path) + ft_strlen(strs[i]);
		if (ft_strlcat(path, strs[i], path_len + 1) != path_len)
			return (free(path), NULL);
	}
	return (path);
}
