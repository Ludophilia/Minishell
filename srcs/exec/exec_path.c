/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:18:44 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/07 23:35:04 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exc_check_full_path_access(int verbose, char *path, t_core *core)
{
	struct stat		statb;
	int				acc_sta;
	int				sta_sta;

	acc_sta = access(path, X_OK);
	if (verbose && acc_sta == -1 && exc_err_path(true, path, core))
		return (false);
	sta_sta = stat(path, &statb);
	if (verbose && sta_sta == -1 && exc_err_path(false, path, core))
		return (false);
	if (acc_sta == 0 && S_ISREG(statb.st_mode))
		return (true);
	if (verbose)
		exc_err_pathg(path, "Is not a file", core);
	return (false);
}

static int	exc_build_path_and_check_access(int i, char **paths, char **argv,
	t_core *core)
{
	int		status;
	char	*path;

	path = exc_build_path((char *[]){paths[i], "/", argv[0], 0});
	if (path == NULL)
		return (-1);
	status = exc_check_full_path_access(false, path, core);
	if (status == true)
	{
		*argv = (free(*argv), path);
		return (true);
	}
	free(path);
	return (false);
}

static int	exc_search_dir_and_check_access(char **argv, char **envp,
	t_core *core)
{
	char	**paths;
	int		i;
	int		status;

	if (exc_load_path(envp, &paths) == -1)
		return (-1);
	if (paths == NULL && exc_err_cmd(*argv, core))
		return (false);
	i = -1;
	while (paths[++i])
	{
		status = exc_build_path_and_check_access(i, paths, argv, core);
		if (status == -1 && utl_free_strs(0, paths))
			return (-1);
		if (status == true && utl_free_strs(0, paths))
			return (true);
	}
	return (utl_free_strs(0, paths), exc_err_cmd(*argv, core), false);
}

int	exc_check_path(char **argv, char **envp, t_core *core)
{
	int		status;

	if (**argv == 0 && exc_err_ecmd(**argv, core))
		return (false);
	if (ft_strchr(*argv, '/'))
	{
		if (exc_check_full_path_access(true, *argv, core))
			return (true);
		else
			return (false);
	}
	status = exc_search_dir_and_check_access(argv, envp, core);
	if (status == -1)
		return (-1);
	if (status == true)
		return (true);
	return (false);
}
