/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isatty.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 19:39:12 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/30 19:56:44 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int	main(void)
{
	int	fd;

	fd = open("file", O_CREAT, 0600);
	if (fd == -1)
		return (1);
	printf("isatty(0) = %i\n", isatty(0));
	printf("isatty(1) = %i\n", isatty(1));
	printf("isatty(2) = %i\n", isatty(2));
	printf("isatty(%i) = %i\n", fd, isatty(fd));
	close(fd);
	unlink("file");
	return (0);
}