/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigaction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 17:38:12 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/14 18:43:05 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void	handler1(int signal_number)
{
	printf("SIGCONT (#%i) handler 1\n", signal_number);
}

void	handler2(int signal_number)
{
	printf("SIGCONT (#%i) handler 2\n", signal_number);
}

int	main(void)
{
	struct sigaction	action;
	struct sigaction	old_action;

	action.sa_handler = handler1;
	if (sigaction(SIGCONT, &action, NULL) == -1)
		return (1);
	action.sa_handler = handler2;
	if (sigaction(SIGCONT, &action, &old_action) == -1)
		return (2);
	(*old_action.sa_handler)(SIGCONT);
	(*action.sa_handler)(SIGCONT);
	printf("[%i] Waiting for signal...\n", getpid());
	while (1)
		;
	return (0);
}
