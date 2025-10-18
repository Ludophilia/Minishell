/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:18:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/18 18:27:55 by jegerman         ###   ########.fr       */
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
		paths = NULL;
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

static int	exc_check_access(char *path)
{
	struct stat	statb;
	int			acc_ex;
	int			sta_ex;

	acc_ex = fmgr_access(path, X_OK);
	sta_ex = stat(path, &statb);
	if (acc_ex == -1
		|| (sta_ex == -1 && ft_eprintf(ERR_PTH, path, strerror(errno))))
		return (0);
	if (acc_ex == 0 && S_ISREG(statb.st_mode))
		return (1);
	ft_eprintf(ERR_PTH, path, "Is not a file");
	return (0);
}

static int	exc_check_access_for_path(int i, char **paths, char **argv)
{
	int		check_rtv;
	char	*path;

	path = exc_build_path((char *[]){paths[i], "/", argv[0], 0});
	if (path == NULL)
		return (-1);
	check_rtv = access(path, X_OK);
	if (check_rtv == 0)
	{
		*argv = (free(*argv), path);
		return (1);
	}
	return (free(path), 0);
}

int	exc_check_path(char **argv, char **envp)
{
	char	**paths;
	int		checkp_rtv;
	int		i;

	if (ft_strchr(*argv, '/'))
		return (exc_check_access(*argv));
	paths = exc_load_paths(envp);
	if (paths == NULL)
	{
		ft_eprintf(ERR_CMD, *argv);
		return (0);
	}
	i = -1;
	while (paths[++i])
	{
		checkp_rtv = exc_check_access_for_path(i, paths, argv);
		if (checkp_rtv == -1 && utl_free_strs(0, paths))
			return (-1);
		if (checkp_rtv == 1 && utl_free_strs(0, paths))
			return (1);
	}
	ft_eprintf(ERR_CMD, *argv);
	utl_free_strs(0, paths);
	return (0);
}
