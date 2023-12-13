/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 13:30:29 by jgermany          #+#    #+#             */
/*   Updated: 2023/04/26 20:49:29 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_ex.h"

/* Level0 - What does exit do exactly? Process termination? So, once exit() is
evaluated, the process dies and no other statement is processed? And what
about the status code? Don't hesitate to use echo $? to get it...*/
void	exit_level0(unsigned do_it)
{
	if (!do_it)
		return ;
	printf("[%i] Start of exit_level0()\n", getpid());
	exit(864);
	printf("[%i] End of exit_level0() that will never be evaluated\n",
		getpid());
}

/* Level1 - use of exit with fork to illustrate how status code is passed from
a child to its parent... exit(3) wstatus encompasses way more than the status 
code, hence the following use of left shifts and bit masks */
int		exit_level1(unsigned do_it)
{
	pid_t	child_pid;
	int		child_status;

	if (!do_it)
		return (0);
	child_pid = fork();
	if (child_pid == -1)
		return (-1);
	if (child_pid == 0)
	{
		printf("[%i, %i] Child exclusive statement\n", getpid(), getppid());
		exit(69); // NOICE ( ͡° ͜ʖ ͡°)
	}
	else
	{
		wait(&child_status);
		printf("[%i] Child returned with status_code = %i\n",
			getpid(), (child_status >> 8) & 0xFF); 
	}
	return (0);
}

/* main() has a status code of 42, as only the least significant byte matters 
 (status & 0x000000FF) */
int	main(void)
{
	set_up();
	exit_level0(0);
	if (exit_level1(1) == -1)
		return (EXIT_FAILURE);
	return (298);
}
