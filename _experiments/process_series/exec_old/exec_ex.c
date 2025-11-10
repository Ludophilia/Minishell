/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 10:52:10 by jgermany          #+#    #+#             */
/*   Updated: 2023/04/28 19:37:10 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wait.h>
#include <stdlib.h>
#include <unistd.h>

/* Level0a - Just a simple pick up of execve(2) the og of the exec family.
execve takes the full path to the program or script to execute, an array of 
strings staring with the prog name, then the args to the program, then null,
and an array of environment variables... */
int	execve_level0a(unsigned letsgo, char *prgpth, char **args, char **envp)
{
	if (!letsgo)
		return (0);
	if (execve(prgpth, args, envp) == -1)
		return (-1);
	write(1, "This should not appear on stdout!\n", 34);
	return (0);
}

/* Level0b - execvpe(3) is an exec family function built on top of execve(2).
The main difference is that the arguemnt pathname of execve is replaced by 
filename in execvpe. if filename does not contain any /, it will be searched 
against any directory specified in PATH. 
That's one problem we will have to deal with with pipex btw... */
int	execvpe_level0b(unsigned letsgo, char *prg, char **args, char **envp)
{
	if (!letsgo)
		return (0);
	if (execvpe(prg, args, envp) == -1)
		return (-1);
	write(1, "Again, this should not appear on stdout!\n", 34);
	return (0);
}

/* Level1 - incubate_level1 is a funtion that creates a child process to avoid 
killing the main process (and subsequent exec calls...) while testing those 
exec functions... */
int	incubate_level1(unsigned letsgo, int (*exec)(unsigned, char *, char **,
	char **), char *prgpth, char **args, char **envp)
{
	pid_t	pid;
	int		wstatus;
	
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		if (exec(letsgo, prgpth, args, envp) == -1)
			exit(EXIT_FAILURE);
		exit(42);
	}
	else
	{
		if (wait(&wstatus) == -1)
			return (-1);
		if ((wstatus >> 8 & 0xFF) == EXIT_FAILURE)
			return (-1);
		return (0);
	}
}

/* New show airing RIGHT NOW: Keeping up with the execs (exec(l|v)[p][e])! */
int	main(int argc, char **argv, char **envp)
{
	if (incubate_level1(
			1, 
			execve_level0a,
			"/usr/bin/ls",
			(char *[]){"/usr/bin/ls", "-mp", NULL}, 
			envp
		) == -1)
		return (-1);
	if (incubate_level1(
			1, 
			execve_level0a,
			"level0/env/env_ex.out",
			(char *[]){"level0/env/env_ex.out", NULL}, 
			(char *[]){"ENV=league", "USER=aketchum", "PIKA=/c/huu/uuuu", NULL}
		) == -1)
		return (-1);
	if (incubate_level1(
			1, 
			execvpe_level0b,
			"echo",
			(char *[]){"echo", "ALLONS BONG", NULL}, 
			NULL
		) == -1)
		return (-1);
	return (0);
}
