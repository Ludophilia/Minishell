/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/14 15:13:16 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(char *line)
{
	if (ft_strlen(line) == 4 && ft_strncmp("exit", line, 4) == 0)
	{
		printf("exit\n");
		return (1);
	}
	return (0);
}

int	interf_loop_prompt(void)
{
	char	*line;

	while (1)
	{
		line = readline(INTERF_PROMPT);
		if (line == NULL)
		{
			printf("[DEBUG] Is that really an error?\n");
			perror("[DEBUG]");
			return (-1);
		}
		if (*line != 0)
			add_history(line);
		
		if (builtin_exit(line))
			break ;
		free(line);
	}
	return (0);
}

int	main(void)
{
	if (interf_loop_prompt() == -1)
		return (1);
	return (0);
}
