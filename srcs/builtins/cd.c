/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:05:44 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/18 14:16:16 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Détermine le chemin cible en fonction des arguments
static char	*cd_get_path(t_core *core, char **argv)
{
	if (!argv[1] || ft_strncmp(argv[1], "--", 3) == 0)
		return (env_get(core->env, "HOME"));
	if (argv[1][0] == '-' && argv[1][1] == '-' && argv[1][2] != '\0')
	{
		ft_eprintf("cd: %s: invalid option\n", argv[1]);
		return (NULL);
	}
	if (ft_strncmp(argv[1], "-", 2) == 0)
		return (env_get(core->env, "OLDPWD"));
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
	{
		if (cmd->argv[1] && cmd->argv[1][0] == '-' && cmd->argv[1][1] != '-')
			ft_eprintf(ERR_GNR, "cd: OLDPWD not set");
		else if (!cmd->argv[1] || ft_strncmp(cmd->argv[1], "--", 3) == 0)
			ft_eprintf(ERR_GNR, "cd: HOME not set");
		return (core->exit = 1, 1);
	}
	if (cmd->argv[1] && ft_strncmp(cmd->argv[1], "-", 2) == 0
		&& ft_strlen(cmd->argv[1]) == 1)
		ft_putendl_fd(path, STDOUT_FILENO);
	if (chdir(path) == -1)
		return (perror("minishell: cd"), core->exit = 1, 1);
	return (cd_update_env(core, oldpwd));
}
