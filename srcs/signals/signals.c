/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:59:28 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/17 21:04:15 by ntahri           ###   ########.fr       */
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
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (-1);
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		return (-1);
	return (0);
}
