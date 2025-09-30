/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:18:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/30 18:28:11 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Logic to check from the envp if a file can be executed
// Execve


static char	**exc_load_env_paths(char **envp)
{
	char	**paths;
	int		is_dflt;
	int		i;

	is_dflt = 1;
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

// Builds the full path for the program, if needed.
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
		{
			free(path);
			return (NULL);
		}
	}
	return (path);
}

static int	exc_free_strs(int from_id, char **strs)
{
	int	i;

	if (strs == NULL)
		return (-1);
	i = 0;
	while (strs[from_id + i])
		free(strs[from_id + i++]);
	free(strs);
	return (1);
}

// returns 0. When a file is found and executable. 
int	exc_check_path(char **argv, char **envp)
{
	char	**env_paths;
	char	*prg_pth;
	int		i;

	if (ft_strchr(*argv, '/'))
		return (fmgr_access(*argv, X_OK));
	env_paths = exc_load_env_paths(envp);
	if (env_paths == NULL)
		return (-1);
	i = -1;
	while (env_paths[++i])
	{
		prg_pth = exc_build_path((char *[]){env_paths[i], "/", argv[0], 0});
		if (prg_pth == NULL)
			return (exc_free_strs(0, env_paths), -1);
		if (access(prg_pth, X_OK) == 0)
		{
			free(*argv);
			*argv = prg_pth;
			return (exc_free_strs(0, env_paths), 0);
		}
		free(prg_pth);
	}
	return (ft_eprintf(ERR_CMD, *argv), exc_free_strs(0, env_paths), -1);
}
