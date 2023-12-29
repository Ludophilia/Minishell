/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:59:28 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/29 15:46:46 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigmgr_gen_handler(int sig_num)
{
	g_exit_status = sig_num + (0x8 << 4);
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
	t_sigaction	gen_sigaction;
	t_sigaction	ign_sigaction;

	ft_bzero(&gen_sigaction, sizeof(t_sigaction));
	ft_bzero(&ign_sigaction, sizeof(t_sigaction));
	gen_sigaction.sa_handler = sigmgr_gen_handler;
	ign_sigaction.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &gen_sigaction, NULL) == -1
		|| sigaction(SIGQUIT, &ign_sigaction, NULL) == -1)
		return (-1);
	return (0);
}
