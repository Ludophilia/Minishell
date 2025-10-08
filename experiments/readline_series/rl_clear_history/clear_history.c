/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 13:17:17 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/23 16:42:18 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

int	main(void)
{
	HIST_ENTRY**	my_history;
	char			*user_line;
	int				i;

	// using_history();
	i = -1;
	while (++i < 5)
	{
		user_line = readline("my custom prompt > ");
		if (user_line == NULL)
			return (1);
		add_history(user_line);
		printf("readline returned: '%s'\n", user_line);
		free(user_line);
	}
	my_history = history_list();
	if (my_history == NULL)
		return (2);
	clear_history();
	i = -1;
	while (my_history[++i])
		printf("#%i: '%s'\n", i + history_base, my_history[i]->line);
	return (0);
}
