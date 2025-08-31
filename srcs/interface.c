/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2025/08/31 19:46:47 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ui_loop_prompt(t_core *core)
{
	char	*line;

	(void)core;
	while (1)
	{
		line = readline(UI_PROMPT);
		if (line == NULL && bi_exit(line) == 0)
			return (0);
		if (*line != 0)
			add_history(line);

		// 13/03/24 - So this is where we are stuck now. The parser...
		if (*line != 0 && lex_parse_line(line) == -1)
			return (-1);

		// 31/08 - Really those builtins? Out there?
		if (ft_strlen(line) == 4 && !ft_strncmp("exit", line, 4))
			return (bi_exit(line), 0);
		free(line);
	}
	return (0);
}
