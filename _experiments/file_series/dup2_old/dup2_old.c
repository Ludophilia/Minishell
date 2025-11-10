/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup2_old.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 21:26:41 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/02 15:41:10 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define BUFFER_SIZE 50

int gpt_redirect(void)
{
    char 	*buffer;
    int 	fd[2];

	buffer = calloc(BUFFER_SIZE, sizeof(char));
    pipe(fd);
    if (fork() == 0)
	{
        close(fd[0]);
        printf("[child %i]\n", getpid());
		// fflush(stdout);
        dup2(fd[1], STDOUT_FILENO);
        printf("Hello, gpt world!");
        close(fd[1]);
    }
	else
	{
		wait(0);
        close(fd[1]);
        read(fd[0], buffer, 16);
        printf("[parent %i] '%s'\n\n", getpid(), buffer);
        close(fd[0]);
    }
	free(buffer);
    return (0);
}

int	main(void)
{
	gpt_redirect();
	return (0);
}