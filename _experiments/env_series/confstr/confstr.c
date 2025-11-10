/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confstr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 18:26:11 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/02 13:38:04 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	char	buffer[128];
	size_t	size;
	
	size = confstr(_CS_PATH, NULL, 0);
	if (size == 0)
		return (1);
	printf("Will take %lu bytes.\n", size);
	if (confstr(_CS_PATH, buffer, 128) == 0)
		return (1);
	printf("PATH='%s'\n", buffer);
    return (0);
}
