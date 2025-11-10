/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:15:50 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/30 18:49:40 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#define SLEEPTIME 3

void print_rusage(struct rusage *rusage)
{
	printf("rusage:\n");
	printf("\tuser CPU time used = %lis\n", rusage->ru_utime.tv_sec);
	printf("\tuser CPU time used = %lius\n", rusage->ru_utime.tv_usec);
	printf("\tsystem CPU time used = %lis\n", rusage->ru_stime.tv_sec);
	printf("\tintegral shared memory size = %li\n", rusage->ru_ixrss);
	printf("\tintegral unshared data size = %li\n", rusage->ru_idrss);
	printf("\tintegral unshared stack size = %li\n", rusage->ru_isrss);
	printf("\tpage reclaims (soft page faults) = %li\n", rusage->ru_minflt);
	printf("\tpage faults (hard page faults) = %li\n", rusage->ru_majflt);
	printf("\tswaps = %li\n", rusage->ru_nswap);
	printf("\tblock input operations = %li\n", rusage->ru_inblock);
	printf("\tblock output operations = %li\n", rusage->ru_oublock);
	printf("\tIPC messages sent = %li\n", rusage->ru_msgsnd);
	printf("\tIPC messages received = %li\n", rusage->ru_msgrcv);
	printf("\tsignals received = %li\n", rusage->ru_nsignals);
	printf("\tvoluntary context switches = %li\n", rusage->ru_nvcsw);
	printf("\tinvoluntary context switches = %li\n", rusage->ru_nivcsw);
}

int	main(void)
{
	pid_t			fork_pid;
	int				wstatus;
	struct rusage	rusage;

	fork_pid = fork();
	if (fork_pid == -1)
		return (1);
	else if (fork_pid == 0
		&& printf("sleeping for %i seconds...\n", SLEEPTIME) > -1
		&& sleep(SLEEPTIME) == 0)
		exit(2);
	else
	{
		if (wait3(&wstatus, 0, &rusage) == -1)
			return (2);
		printf("child exited with %i.\n", (wstatus >> 8) & 0xFF);
		print_rusage(&rusage);
	}
	return (0);
}