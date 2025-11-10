/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 15:27:08 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/28 17:44:08 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#define FILENAME "file"

int		main(void)
{
	int	fd;

	fd = open(FILENAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
	if (fd == -1 || close(fd) == -1 || unlink(FILENAME) == -1)
	{
		dprintf(2, "Something wrong happened.\n");
		return (1);
	}
	return (0);
}
