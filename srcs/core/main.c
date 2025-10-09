/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/09 04:06:32 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include "minishell.h"

uint32_t g_exit_status = 0; // signals stops me from removing it...

int main(int argc, char **argv, char **envp)
{
    t_core core;

    if (++argv, --argc != 0)
        return (1);
    core.envp = envp; // provisory
    // (&core)->exit = &g_exit_status;
    if (sig_init_handlers() == -1 || ui_loop_prompt(&core) == -1)
        return (2);
    return (0);
}*/

#include "minishell.h"

uint32_t g_exit_status = 0;

int main(int argc, char **argv, char **envp)
{
    t_core core;

    (void)argv;
    if (argc != 1)
        return (1);
    sig_init_prompt();
    core.flags = FLG_ALL;
    core.envp = envp;
    core.env = dup_env(envp);
    core.cmd_pmax = 0;
    core.cmd_xrdy = 0;
    core.exitv = 0;
    if (ui_loop_prompt(&core) == -1)
    {
        free_env(core.env);
        return (2);
    }
    free_env(core.env);
    return (0);
}
