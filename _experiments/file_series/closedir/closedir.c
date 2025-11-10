/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closedir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:15:09 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/08 19:24:22 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdio.h>

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
		printf("%s\n", dirent->d_name);
		dirent = readdir(dir);
	}
	if (closedir(dir) == -1)
		return (2);
	return (0);
}
