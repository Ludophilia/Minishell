/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/29 16:31:53 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	intf_loop_prompt(t_core *core)
{
	char	*line;

	(void)core;
	while (1)
	{
		line = readline(INTF_PROMPT);
		if (line == NULL && blt_exit(line) == 0)
			return (0);
		if (*line != 0)
			add_history(line);

		// if there is something on the line...
		if (*line != 0 && psr_parse_line(line) == -1)
			return (-1);

		if (ft_strlen(line) == 4 && !ft_strncmp("exit", line, 4)
			&& blt_exit(line) == 0)
			return (0);
		free(line);
	}
	return (0);
}
