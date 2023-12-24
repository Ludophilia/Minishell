/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/24 18:50:33 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// What a nigga should do?


// - From a line gotten via readline...
//		- Extract the command (and its flags and arguments)
//			- spaces at the beginning? (as defined by isspace(3))
//			- from the first character that is not: <, <<, >>, >, (, ), |, ", ', ||, &&)
//			- to the next character that is null or something that is: <, <<, >>, >, (, ), |, ", ', ||, &&)

//		- (start simple)

// 		- Extract the redirections

void	extract_command(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
		;
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

		// What should be done here?
			// printf("[%i] line = '%s'\n", getpid(), line);
		// if there is something on the line...
			// break down the line
			// 
		if (ft_strlen(line) == 4 && !ft_strncmp("exit", line, 4)
			&& blt_exit(line) == 0)
			return (0);
		free(line);
	}
	return (0);
}

