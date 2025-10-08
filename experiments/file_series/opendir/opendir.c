/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opendir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:15:09 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/08 19:15:39 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdio.h>

int	main(void)
{
	DIR	*dir;

	dir = opendir(".");
	if (dir == NULL)
	{
		perror(NULL);
		return (1);
	}
	dprintf(2, "Without closedir, there will be leaks!\n");
	return (0);
}
