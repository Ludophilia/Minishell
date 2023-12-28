/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/28 20:10:59 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// (27/12) What to do?

// - Start implementing exit status management. The exit status 
//	of ^C should be 130.

// Why do you extract from the line commands, arguments and file paths information?

// 	- The GOAL is to execute programs (commands) with some options (i/o 
// redirected to a file or a pipe). 

// - To realize this we need to pass data around different modules and thus
// to create a structure to host commands and their information or an array
// of those structures 
	// - So, what does the structure contain?
		// - the command obviously (name or absolute path)
		// - the file descriptors for the input (if needed)
			// - Why not store the path to a file and open it later
			// in the process?
			// 		- Not every redirection with <, <<, >>, > leads to a command...
		// - the file descriptors for the output (if needed)
		// - environment variables envp (in case)
		// - ???



int	psr_parse_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (psr_is_type(TY_SPAC, line + i) == 1)
			i += 1;
		else if (psr_is_type(TY_SPEC, line + i) == 0
			&& psr_extract_cmd(&i, line + i) >= 0)
			i += 0;
		else if (psr_is_type(TY_RDIN, line + i) == 1
			&& psr_extract_path("inpath", &i, line + i + 1) >= 0)
			i += 1;
		else if (psr_is_type(TY_RDOUT, line + i) == 1
			&& psr_extract_path("outpath", &i, line + i + 1) >= 0)
			i += 1;
		else if (psr_is_type(TY_RDHRD, line + i) == 1
			&& psr_extract_path("here", &i, line + i + 2) >= 0)
			i += 2;
		else if (psr_is_type(TY_RDAPN, line + i) == 1
			&& psr_extract_path("apnpath", &i, line + i + 2) >= 0)
			i += 2;
		else
		{
			printf("That will be the error case\n");
			// return (-1);
			break ;
		}
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
