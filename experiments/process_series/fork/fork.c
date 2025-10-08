/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 14:14:50 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/28 18:44:14 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(void)
{
	pid_t	pid;

	printf("[%i]\t[%i]\tmain() starts!\n", getppid(), getpid());
	pid = fork(); 
	if (pid == -1)
		return (1);
	if (pid == 0)
		printf("[%i]\t[%i]\tChild process exclusive statement\n",
			getppid(), getpid());
	else
		printf("[%i]\t[%i]\tParent process exclusive statement\n",
			getppid(), getpid());
	printf("[%i]\t[%i]\tmain() is about to end.\n", getppid(), getpid());
	return (0);
}
