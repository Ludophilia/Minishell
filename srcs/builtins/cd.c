/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:05:44 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/18 18:36:08 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Détermine le chemin cible en fonction des arguments
static char	*cd_get_path(t_core *core, char **argv)
{
	char	*path;

	if (!argv[1] || ft_strncmp(argv[1], "--", 3) == 0)
	{
		path = env_get(core->env, "HOME");
		if (!path || path[0] == '\0')
			return (NULL);
		return (path);
	}
	if (argv[1][0] == '-' && argv[1][1] == '-' && argv[1][2] != '\0')
	{
		ft_eprintf("cd: %s: invalid option\n", argv[1]);
		return (NULL);
	}
	if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		path = env_get(core->env, "OLDPWD");
		if (!path || path[0] == '\0')
			return (NULL);
		return (path);
	}
	return (argv[1]);
}

// Met à jour les variables d'environnement PWD et OLDPWD après un cd réussi
static int	cd_update_env(t_core *core, char *oldpwd)
{
	char	newpwd[PTH_MAX];

	if (!getcwd(newpwd, sizeof(newpwd)))
	{
		perror("cd: getcwd");
		core->exit = 1;
		return (1);
	}
	env_set(&core->env, "OLDPWD", oldpwd);
	env_set(&core->env, "PWD", newpwd);
	return (0);
}

// Gère les erreurs de chemin
static int	cd_handle_path_error(t_core *core, char **argv)
{
	if (argv[1] && argv[1][0] == '-' && argv[1][1] != '-')
		ft_eprintf(ERR_GNR, "cd: OLDPWD not set");
	else if (!argv[1] || (argv[1][0] == '-'
		&& argv[1][1] == '-' && argv[1][2] == '\0'))
		ft_eprintf(ERR_GNR, "cd: HOME not set");
	if (argv[1] && argv[1][0] == '-' && argv[1][1] == '-' && argv[1][2] != '\0')
		return (core->exit = 2, 2);
	return (core->exit = 1, 1);
}

int	bi_cd(t_core *core, t_cmd *cmd)
{
	char	oldpwd[PTH_MAX];
	char	*path;

	if (cmd->argc > 2)
		return (ft_eprintf(ERR_BMNY, "cd"), core->exit = 1, 1);
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		return (perror("cd: getcwd"), core->exit = 1, 1);
	path = cd_get_path(core, cmd->argv);
	if (!path)
		return (cd_handle_path_error(core, cmd->argv));
	if (cmd->argv[1] && ft_strncmp(cmd->argv[1], "-", 2) == 0 &&
		ft_strlen(cmd->argv[1]) == 1)
		ft_putendl_fd(path, STDOUT_FILENO);
	if (chdir(path) == -1)
	{
		ft_eprintf("minishell: cd: %s: %s\n", path, strerror(errno));
		core->exit = 1;
		return (1);
	}
	return (cd_update_env(core, oldpwd));
}

