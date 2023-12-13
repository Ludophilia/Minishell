/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 13:45:30 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/02 14:08:11 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

int	main(void)
{
	char	**envs;

	envs = (char *[]){"SHELL", "PATH", "OLDPWD", "HOME", "USER",
		"SHLVL", "SHINJI", 0};
	while (*envs)
	{
		printf("%s: %s\n", *envs, getenv(*envs));
		envs++;
	}
	return (0);
}
