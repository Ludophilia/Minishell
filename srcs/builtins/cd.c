/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:05:44 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/09 04:06:45 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Détermine le chemin cible en fonction des arguments
static char *cd_get_path(t_core *core, char **argv)
{
    if (!argv[1]) // cas: cd → HOME
        return get_env(core->env, "HOME");
    if (ft_strncmp(argv[1], "-", 2) == 0) // cas: cd -
        return get_env(core->env, "OLDPWD");
    return argv[1]; // cas général
}

// Met à jour les variables d'environnement PWD et OLDPWD après un cd réussi
static int cd_update_env(t_core *core, char *oldpwd)
{
    char newpwd[PATH_MAX];

    if (!getcwd(newpwd, sizeof(newpwd)))
    {
        perror("cd: getcwd");
        g_exit_status = 1;
        return (1);
    }
    set_env(&core->env, "OLDPWD", oldpwd);
    set_env(&core->env, "PWD", newpwd);
    return (0);
}

int bi_cd(t_core *core, t_cmd *cmd)
{
    char oldpwd[PATH_MAX];
    char *path;

    if (!getcwd(oldpwd, sizeof(oldpwd)))
        return (perror("cd: getcwd"), g_exit_status = 1, 1);

    path = cd_get_path(core, cmd->argv);
    if (!path)
    {
        ft_putendl_fd("minishell: cd: HOME/OLDPWD not set", STDERR_FILENO);
        return (g_exit_status = 1, 1);
    }

    if (cmd->argv[1] && ft_strncmp(cmd->argv[1], "-", 2) == 0)
        ft_putendl_fd(path, STDOUT_FILENO);

    if (chdir(path) == -1)
        return (perror("minishell: cd"), g_exit_status = 1, 1);

    return cd_update_env(core, oldpwd);
}
