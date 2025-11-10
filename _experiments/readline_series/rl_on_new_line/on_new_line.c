/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_new_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 13:17:17 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/24 15:05:31 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

# define LINES 1

int	main(void)
{
	char						*user_line;
	int							i;

	i = -1;
	while (++i < LINES)
	{
		user_line = readline("my custom prompt > ");
		if (user_line == NULL)
			return (1);
		free(user_line);
	}
	rl_on_new_line();
	rl_redisplay();
	return (0);
}
