/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 18:26:11 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/02 13:24:30 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	main(int argc, char **argv, char **envp)
{	
	(void)argc;
	(void)argv;
	while (*envp)
		printf("%s\n", *envp++);
    return (0);
}
