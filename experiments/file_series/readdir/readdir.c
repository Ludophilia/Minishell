/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readdir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:15:09 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/08 19:16:41 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdio.h>
#include <stdint.h>

char *main_get_file_type(uint8_t filetype)
{
	switch (filetype)
	{
		case DT_BLK:
			return ("block device");
		case DT_CHR:
			return ("character device");
		case DT_DIR:
			return ("directory");
		case DT_FIFO:
			return ("named pipe (FIFO)");
		case DT_LNK:
			return ("symbolic link");
		case DT_REG:
			return ("regular file");
		case DT_SOCK:
			return ("UNIX domain socket");
		case DT_UNKNOWN:
			return ("not determined");
		default:
			return ("unknown");
	}
}

void	main_print_dirent(struct dirent *dirent)
{
	printf("%s\n", dirent->d_name);
	printf("\tinode number: %lu\n", dirent->d_ino);
	printf("\tcurrent location in directory stream: %lu\n", dirent->d_off);
	printf("\tsize of the record: %u bytes\n", dirent->d_reclen);
	printf("\tfile type: %u or %s\n",
		dirent->d_type, main_get_file_type(dirent->d_type));	
}

int	main(void)
{
	DIR				*dir;
	struct dirent	*dirent;

	dir = opendir(".");
	if (dir == NULL)
	{
		perror(NULL);
		return (1);
	}
	dirent = readdir(dir);
	while (dirent)
	{
		main_print_dirent(dirent);
		dirent = readdir(dir);
	}
	dprintf(2, "Without closedir, there will be leaks!\n");
	return (0);
}
