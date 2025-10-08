/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getcwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 19:39:12 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/30 20:15:36 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 255

// int	main(void)
// {
// 	char	*pcwd;

// 	pcwd = 0;
// 	pcwd = getcwd(pcwd, BUFFER_SIZE);
// 	if (pcwd == NULL)
// 		return (1);
// 	printf("cwd = %s\n", pcwd);
// 	free(pcwd);
// 	return (0);
// }

int	main(void)
{
	char	cwd[BUFFER_SIZE];
	char	*pcwd;

	pcwd = getcwd(cwd, BUFFER_SIZE);
	if (pcwd == NULL)
		return (1);
	printf("cwd = %s\n", pcwd);
	return (0);
}
