/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/26 19:19:54 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// What a nigga should do?


// - From a line gotten via readline...
//		- Extract the command (and its flags and arguments)
//			- spaces at the beginning? (as defined by isspace(3))
//			- from the first character that is not: <, <<, >>, >,
//				(, ), |, ", ', ||, &&)
//			- to the next character that is null or something that is:
//				<, <<, >>, >, (, ), |, ", ', ||, &&)

//		- (start simple)

// 		- Extract the redirections


enum e_typ
{
	TY_SPAC,
	TY_RDIN,
	TY_SPEC,
	TY_RDOUT
};


int	psr_is_type(enum e_typ type, char *c)
{
	int	i;

	if (c == NULL)
		return (-1);
	i = 0;
	while (type == TY_SPEC && SPECIAL_CHARS[i])
	{
		// printf("SPECIAL_CHARS[i] = '%c'\n", SPECIAL_CHARS[i]);		
		if (SPECIAL_CHARS[i++] == *c)
			return (1);
	}
	if (type == TY_SPAC && (*c == ' ' || (*c >= '\t' && *c <= '\r')))
		return (1);
	else if (type == TY_RDIN && (*c == '<' && *(c + 1) != '<'))
		return (1);
	return (0);
}

// Command can start virtually with everything...

int	psr_extract_cmd(int *i, char *line)
{
	char	*cmd;
	int		j;

	if (i == NULL)
		return (-1);
	j = 0;
	while (line[j] && psr_is_type(TY_SPEC, line + j) == 0)
		j++;
	cmd = ft_substr(line, 0, j);
	if (cmd == NULL)
		return (-1);
	*i += j;
	printf("cmd = '%s'\n", cmd);
	free(cmd);
	return (0);
}

int	psr_extract_path(int *i, char *line)
{
	char	*path;
	int		j;
	int		k;

	if (i == NULL)
		return (-1);
	j = 0;
	while (line[j] && psr_is_type(TY_SPAC, line + j))
		j++;
	k = j;
	while (line[k] && !psr_is_type(TY_SPEC, line + k)
		&& !psr_is_type(TY_SPAC, line + k))
		k++;
	path = ft_substr(line, j, k - 1);
	if (path == NULL)
		return (-1);
	*i += k + 1;
	printf("path = '%s'\n", path);
	free(path);
	return (0);
}

// #define SPECIAL_CHARS "<>()|\"'&"

// (line[i] == '<' && line[i + 1] != '<')

int	psr_parse_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{

		if (psr_is_type(TY_SPAC, line + i))
		{
			// printf("A space\n");
			i++;
		}
		else if (!psr_is_type(TY_SPEC, line + i))
		{
			if (psr_extract_cmd(&i, line + i) == -1)
				return (-1);
		}
		else if (psr_is_type(TY_RDIN, line + i))
		{
			if (psr_extract_path(&i, line + i + 1) == -1)
				return (-1);
		}
		else
			break ;

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

