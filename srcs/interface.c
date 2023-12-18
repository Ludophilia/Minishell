/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/18 18:32:48 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	intf_loop_prompt(void)
{
	char	*line;

	while (1)
	{
		line = readline(INTF_PROMPT);
		if (line == NULL && blt_exit(line) == 0)
			return (0);
		if (*line != 0)
			add_history(line);

		// What should be done here?
		// printf("[%i] line = '%s'\n", getpid(), line);

		if ((ft_strlen(line) == 4 && !ft_strncmp("exit", line, 4))
			&& blt_exit(line) == 0)
			return (0);
		free(line);
	}
	return (0);
}

