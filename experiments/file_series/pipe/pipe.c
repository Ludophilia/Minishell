/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 15:13:32 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/08 14:11:07 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>

#define PIPE_MESSAGE "Hello pipe"
#define PIPE_MESSAGE_SIZE 11

int main(void)
{
	char	buffer[128];
	int		fds[2];

	if (memset(buffer, 0, 128 * sizeof(char)) == NULL || pipe(fds) == -1)
		return (1);
	if (write(STDOUT_FILENO, buffer, 128) == -1)
		return (2);
	if (write(fds[0], PIPE_MESSAGE, PIPE_MESSAGE_SIZE) == -1)
		write(STDERR_FILENO, "Don't write on the read end of the pipe!\n", 42);
	if (write(fds[1], PIPE_MESSAGE, PIPE_MESSAGE_SIZE) == -1)
		return (3);
	if (read(fds[1], buffer, PIPE_MESSAGE_SIZE) == -1)
		write(STDERR_FILENO, "Don't read the write end of the pipe!\n", 39);
	if (read(fds[0], buffer, PIPE_MESSAGE_SIZE) == -1
		|| write(STDOUT_FILENO, "From pipe: '", 13) == -1
		|| write(STDOUT_FILENO, buffer, PIPE_MESSAGE_SIZE) == -1
		|| write(STDOUT_FILENO, "'\n", 3) == -1)
		return (4);
	return (0);
}
