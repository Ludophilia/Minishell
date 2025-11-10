/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 13:56:38 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/26 14:39:19 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libs/libft/includes/libft.h"
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	char	*hello;
	char	*substr;

	if (argc != 2 && dprintf(2, "hey boss, can i have one argument please?\n"))
		return (1);
	hello = "Hello world";
	substr = ft_substr(hello, 0, atoi(argv[1]));
	printf("str = '%s'\n", hello);
	printf("substr = '%s'\n", substr);
	free(substr);
	return (0);
}