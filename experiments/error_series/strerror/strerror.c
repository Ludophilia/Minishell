/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strerror.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 11:56:24 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/02 12:50:58 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int		main(void)
{
	printf("%s\n", strerror(errno));
	if (open("evangelion/characters/rei_ayanami", O_WRONLY) != -1)
		return (1);
	printf("%s\n", strerror(errno));
	printf("%s\n", strerror(-1));
	printf("%s\n", strerror(1));
	printf("%s\n", strerror(2));
	printf("%s\n", strerror(3));
	printf("%s\n", strerror(4));
	printf("%s\n", strerror(5));
	return (0);
}