/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_new.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:59:28 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/06 21:22:28 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void sig_handler_interactive(int sig)
{
    if (sig == SIGINT) // Ctrl+C
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_exit_status = 130;
    }
    else if (sig == SIGQUIT)
    {
        rl_on_new_line();
        rl_redisplay();
    }
}

void sig_init_prompt(void)
{
    struct sigaction sa;

    sa.sa_handler = sig_handler_interactive;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

void sig_init_child(void)
{
    struct sigaction sa;

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}
