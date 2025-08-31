/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:59:28 by jgermany          #+#    #+#             */
/*   Updated: 2025/08/31 19:51:52 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigmgr_gen_handler(int sig_num)
{
	g_exit_status = sig_num + 128;
	if (sig_num == SIGINT)
	{
		write(1, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	sig_init_handlers(void)
{
	t_sigaction	gen_sigact;
	t_sigaction	ign_sigact;

	ign_sigact = (t_sigaction){.sa_handler = SIG_IGN};
	gen_sigact = (t_sigaction){.sa_handler = sigmgr_gen_handler};
	if (sigaction(SIGINT, &gen_sigact, NULL) == -1
		|| sigaction(SIGQUIT, &ign_sigact, NULL) == -1)
		return (-1);
	return (0);
}
