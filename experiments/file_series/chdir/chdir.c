/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 19:39:12 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/30 20:20:48 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 255

int	main(void)
{
	char	cwd[BUFFER_SIZE];
	char	*pcwd;

	pcwd = getcwd(cwd, BUFFER_SIZE);
	if (pcwd == NULL)
		return (2);
	printf("cwd = %s\n", pcwd);
	if (chdir("../../..") == -1)
		return (1);
	pcwd = getcwd(cwd, BUFFER_SIZE);
	if (pcwd == NULL)
		return (2);
	printf("cwd = %s\n", pcwd);
	return (0);
}
