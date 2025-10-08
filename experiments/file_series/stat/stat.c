/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 16:35:56 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/08 17:36:49 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <stdio.h>

char	*main_print_mode(struct stat *file_stats)
{
	switch (file_stats->st_mode & S_IFMT)
	{
		case S_IFBLK:
			return ("block device");
		case S_IFCHR:
			return ("character device");
		case S_IFDIR:
			return ("directory");
		case S_IFIFO:
			return ("FIFO/pipe");
		case S_IFLNK:
			return ("symlink");
		case S_IFREG:
			return ("regular file");
		case S_IFSOCK:
			return ("socket");
		default:
			return ("unknown?");
    }
}

void	main_print_stats(struct stat *file_stats)
{
	printf("ID of device containing file: %lu\n", file_stats->st_dev);
	printf("Inode number: %lu\n", file_stats->st_ino);
	printf("File type and mode: %u or %s\n", file_stats->st_mode,
		main_print_mode(file_stats));
	printf("Number of hard links: %lu\n", file_stats->st_nlink);
	printf("User ID of owner: %u\n", file_stats->st_uid);
	printf("Group ID of owner: %u\n", file_stats->st_gid);
	printf("Device ID: %lu\n", file_stats->st_rdev);
	printf("Total size, in bytes: %li\n", file_stats->st_size);
	printf("Block size, in bytes: %li\n", file_stats->st_blksize);
	printf("Number of 512 B blocks allocated: %li\n", file_stats->st_blocks);
	printf("Time of last access: %li\n", file_stats->st_atime);
	printf("Time of last modification: %li\n", file_stats->st_mtime);
	printf("Time of last status change: %li\n", file_stats->st_ctime);
}

int	main(void)
{
	struct stat	file_stats;

	if (stat("stat.c", &file_stats) == -1)
		return (1);
	main_print_stats(&file_stats);
	return (0);
}
