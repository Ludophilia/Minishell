/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2025/09/07 16:49:55 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ui_loop_prompt(t_core *core)
{
	char	*line;

	while (1)
	{
		line = readline(UI_PROMPT);
		if (line == NULL && bi_exit(line) == 0)
			return (0);
		if (*line != 0)
			add_history(line);

		// 5/09 - Not at the right level anyway...
		if (*line != 0 && psr_parse_line(line, core) == -1)
		{
			free(line);
			continue ; // 7/09 - Find a better way.
		}

		// 31/08 - Will certainly move somewhere else as well
		if (ft_strlen(line) == 4 && !ft_strncmp("exit", line, 4))
			return (bi_exit(line), 0);
		free(line);
	}
	return (0);
}
