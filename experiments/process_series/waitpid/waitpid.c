/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitpid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 20:06:01 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/28 20:24:51 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int	waitpid_level0(unsigned keep_alive)
{
	pid_t	pid;
	int		wstatus;
	pid_t	child_pid;

	if (!keep_alive)
		return (0);
	printf("A waitpid example...\n");
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		printf("[%i]\t[%i]\tAbout to get out of child process in 1s...\n",
			getppid(), getpid());
		usleep(1 * 1000 * 1000);
		exit(42);
	}
	else
	{	
		child_pid = waitpid(pid, &wstatus, 0);
		if (child_pid == -1) 
			return (-1);
		printf("[%i]\t[%i]\tIn parent process with wstatus = %i and wait_ret"
			" of %i!\n", getppid(), getpid(), wstatus, child_pid);
		printf("[%i]\t[%i]\tWIFEXITED returned %s\n", getppid(), getpid(),
			(char *[]){"false", "true"}[WIFEXITED(wstatus)]);
		printf("[%i]\t[%i]\tExit status or WEXITSTATUS: %i\n",
			getppid(), getpid(), WEXITSTATUS(wstatus));
		printf("[%i]\t[%i]\tWIFSIGNALED returned %s\n", getppid(), getpid(),
			(char *[]){"false", "true"}[WIFSIGNALED(wstatus)]);
		printf("[%i]\t[%i]\tTerminating Signal or WTERMSIG: %i\n",
			getppid(), getpid(), WTERMSIG(wstatus));
	}
	return (0);
}

int	waitpid_pong(unsigned keep_alive)
{
	pid_t	pid;
	int		wstatus;

	if (!keep_alive)
		return (0);
	printf("\nAnd now, the wait pong...\n");
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		while (keep_alive)
		{
			printf("[%i]\t[%i]\tOut of child process in %u...\n",
				getppid(), getpid(), keep_alive--);
			if (keep_alive)
				printf("[%i]\t[%i]\tBut first, why not SIGSTOP ourselves?\n",
					getppid(), getpid());
			sleep(1);
			raise(SIGSTOP);
		}
		printf("[%i]\t[%i]\tEnd of child process!\n", getppid(), getpid());
	}
	else
	{	
		if (waitpid(pid, &wstatus, WUNTRACED) == -1)
			return (-1);
		while (WSTOPSIG(wstatus) == SIGSTOP)
		{
			printf("[%i]\t[%i]\tIn parent process!\n\t\tSending SIGCONT to "
			"child process...\n", getppid(), getpid());
			sleep(1);
			kill(pid, SIGCONT);
			if (waitpid(pid, &wstatus, WUNTRACED) == -1)
				return (-1);
		}
		printf("[%i]\t[%i]\tEnd of parent process!\n", getppid(), getpid());
	}
	return (0);
}

int	main(void)
{
	if (waitpid_level0(1) == -1
		|| waitpid_pong(5) == -1)
		return (1);
	return (0);
}
