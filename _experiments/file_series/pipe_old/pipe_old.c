/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 21:26:41 by jgermany          #+#    #+#             */
/*   Updated: 2023/04/19 13:09:58 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 50

int	pipex_lvl0(void)
{
	int		fds[2];
	char	*buffer;

	buffer = calloc(BUFFER_SIZE, sizeof(char));
	if (!buffer || pipe(fds) == -1)
		return (-1);
	if (write(fds[1], "hello pipe\n", 12) == -1)
		return (-1);
	if (read(fds[0], buffer, 12) == 0)
		return (-1);
	if (write(1, buffer, 12) == -1)
		return (-1);
	free(buffer);
	close(fds[0]);
	close(fds[1]);
	return (1);
}

int	pipex_lvl1(void)
{
	int		fds[2];
	int		stdout_fd;
	void	*buffer;

	stdout_fd = dup(STDOUT_FILENO); // Same ex, but with a redirection of stdout
	buffer = calloc(BUFFER_SIZE, sizeof(char));
	if (!buffer || pipe(fds) == -1 || stdout_fd == -1)
		return (-1);
	if (dup2(fds[1], STDOUT_FILENO) == -1)
		return (-1);
	printf("hello (again) pipe\n");
	fflush(stdout); // Without this, the process freezes at printf.
	if (read(fds[0], buffer, 18) == -1)
		return (-1);
	dup2(stdout_fd, STDOUT_FILENO);
	printf("[Buffer]: '%s'\n", (char *)buffer);
	close(fds[0]);
	close(fds[1]);
	return (1);
}

int	main(void)
{
	if (pipex_lvl0() == -1)
		return (-1);
	if (pipex_lvl1() == -1)
		return (-1);
	return (0);
}
