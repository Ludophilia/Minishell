/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 13:17:17 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/22 14:08:50 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

int	main(void)
{
	char	*user_line;

	user_line = readline("my custom prompt > ");
	if (user_line == NULL)
		return (1);
	printf("readline returned: '%s'\n", user_line);
	free(user_line);
	return (0);
}
