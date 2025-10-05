/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:18:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/05 15:14:22 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**exc_load_paths(char **envp)
{
	char	**paths;
	int		is_dflt;
	int		i;

	is_dflt = true;
	i = -1;
	while (envp[++i])
		if (ft_strnstr(envp[i], "PATH", 4) && is_dflt--)
			break ;
	if (is_dflt)
		paths = ft_split(ENV_DFLT_PATH, ':');
	else
		paths = ft_split(envp[i] + 5, ':');
	return (paths);
}

static char	*exc_build_path(char **strs)
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

int	exc_check_path(char **argv, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	if (ft_strchr(*argv, '/'))
		return (fmgr_access(*argv, X_OK));
	paths = exc_load_paths(envp);
	if (paths == NULL)
		return (-1);
	i = -1;
	while (paths[++i])
	{
		path = exc_build_path((char *[]){paths[i], "/", argv[0], 0});
		if (path == NULL && utl_free_strs(0, paths))
			return (-1);
		if (access(path, X_OK) == 0 && utl_free_strs(0, paths))
		{
			*argv = (free(*argv), path);
			return (1);
		}
		free(path);
	}
	return (utl_free_strs(0, paths), ft_eprintf(ERR_CMD, *argv), 0);
}
