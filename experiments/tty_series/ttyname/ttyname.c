/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttyname.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 19:39:12 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/30 20:03:38 by jgermany         ###   ########.fr       */
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
	printf("ttyname(0) = %s\n", ttyname(0));
	printf("ttyname(1) = %s\n", ttyname(1));
	printf("ttyname(2) = %s\n", ttyname(2));
	printf("ttyname(%i) = %s\n", fd, ttyname(fd));
	close(fd);
	unlink("file");
	return (0);
}