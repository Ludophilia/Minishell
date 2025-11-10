/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_ex.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 19:32:19 by jgermany          #+#    #+#             */
/*   Updated: 2023/04/26 20:46:57 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

static void	tear_down_linux(int status, void *pid)
{
	printf("[%i] End of process with status %i.\n", *(int *)pid, status);
}

static void	tear_down(void)
{
	printf("[%i] ...End of process.\n", getpid());
}

static void	exit_handler(void)
{
	printf("[%i] Exiting process...\n", getpid());
}

/* set_up function does some preparatory work in main. THis includes
printing to stdout that the process has started and registering 2 functions
that will be called when the process (or its child) will terminate...
It's important to note that the last registered function will be called first
and so on... */
void	set_up(void)
{
	pid_t	pid;

	pid = getpid();
	printf("[%i] Start of process!\n", pid);
	atexit(tear_down);
	atexit(exit_handler);
	// on_exit(tear_down_linux, &pid);
}
