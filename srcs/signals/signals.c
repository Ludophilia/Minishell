/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:59:28 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/14 22:22:50 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sig_handler_interactive(int sig)
{
	g_sig = sig;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		// 14/10 - Exit status again
		// g_exit_status = 130; // 128 + 2 or 128 + sig
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

int	sig_init_child(void)
{
	struct sigaction	sa;

	if (sigemptyset(&sa.sa_mask) == -1)
		return (-1);
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1
		|| sigaction(SIGQUIT, &sa, NULL) == -1)
		return (-1);
	return (0);
}

int	sig_init_prompt(void)
{
	struct sigaction	sa;

	if (sigemptyset(&sa.sa_mask) == -1)
		return (-1);
	sa.sa_handler = sig_handler_interactive;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1
		|| sigaction(SIGQUIT, &sa, NULL) == -1)
		return (-1);
	return (0);
}
