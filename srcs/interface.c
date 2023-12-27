/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/27 14:52:22 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_parse_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (psr_is_type(TY_SPAC, line + i) == 1)
			i++;
		else if (psr_is_type(TY_SPEC, line + i) == 0
			&& psr_extract_cmd(&i, line + i) >= 0)
			;
		else if (psr_is_type(TY_RDIN, line + i) == 1
			&& psr_extract_path("inpath", &i, line + i + 1) >= 0)
			;
		else
		{
			printf("That will be the error case\n");
			// return (-1);
			break ;
		}


		// printf("line[%i] = %c\n", i, line[i]);


		// if line[i] == <, and line[i + 1] != <, extract file name, maybe open it?
		// if line[i] == < and line[i + 1] == <, extract file name, maybe open it?

		// if line[i] == > and line[i + 1] == >, extract file name, maybe open it?
		// if line[i] == > and line[i + 1] != >, extract file name, maybe open it?

		// if line[i] == \", do something else
		// if line[i] is not a special_char, extract the command

		// 

	}
	return (0);
}

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

