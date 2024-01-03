/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 14:36:28 by jgermany          #+#    #+#             */
/*   Updated: 2024/01/03 18:30:25 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_is_char(t_chartype type, char *line)
{
	int	i;

	if (line == NULL)
		return (-1);
	i = 0;
	while (type == CHR_SPECIAL && SPECIAL_CHARS[i])
	{
		if (SPECIAL_CHARS[i++] == *line)
			return (1);
	}
	if ((type == CHR_SQUOTE && *line == '\'')
		|| (type == CHR_DQUOTE && *line == '\"')
		|| (type == CHR_SPACE && (*line == ' ' || (*line >= 9 && *line <= 13)))
		|| (type == CHR_RDIN && (*line == '<' && *(line + 1) != '<'))
		|| (type == CHR_RDOUT && (*line == '>' && *(line + 1) != '>'))
		|| (type == CHR_HEREDOC && (*line == '<' && *(line + 1) == '<'))
		|| (type == CHR_APPEND && (*line == '>' && *(line + 1) == '>')))
		return (1);
	return (0);
}

int	psr_has_char(t_endtype type, char *line)
{
	int	i;

	i = -1;
	while ((type == END_LINE || type == END_SPECIAL) && line[++i])
	{
		if ((type == END_LINE && line[i + 1] == '\0')
			|| (type == END_SPECIAL && psr_is_char(CHR_SPECIAL, line + i + 1)))
			return (1);
	}
	while ((type == END_SQUOTE || type == END_DQUOTE) && line[++i])
	{
		if ((type == END_SQUOTE && line[i] == '\'')
			|| (type == END_DQUOTE && line[i] == '\"'))
			return (1);
	}
	return (0);
}

// I think I misunderstood the role of quotes (\', \") in command formating...
//		- They are not always at the start of the command, they can be in args too

// Here are my assumptions:
//	- When this function starts, the first character to parse is everything 
//		except (<, >, |) and a space. So it could be a SQUOTE or a DQUOTE,
//		but also anything else.
//  - 

// THE FIRST THING THAT SHOULD BE DONE:
//		- Separating a command from its arguments. Should be done by respecting
// quotes. Within a quote, whitespaces are not a separator anymore...
//		- Don't trim the quotes initially, as more operations involving them is coming. 
// - 
// - 
int	psr_extract_cmd(int *i, char *line)
{
	char	*cmd;
	int		j;
	int		quoted;

	if (i == NULL)
		return (-1);
	// The presence of a " or a ' should do something.
	//	- yes, indicating that a command or an argument is not ending at
	//	the next space ' '...
	//	
	//	- we have to separate commands and arguments first
	//
	// The command part.
	j = 0;
	quoted = 0;
	while (line[j] && (quoted >= 1 || (quoted == 0
		&& psr_is_char(CHR_SPECIAL, line + j) == 0)))
	{
		if (line[j] == '\"' || line[j] == '\'')
		{
			if (line[j] == quoted)
				break ; // WHY? POURQUOI? NAZE?
			quoted = line[j];
			if ((line[j] == '\'' && psr_has_char(END_SQUOTE, line + j) == 0)
				|| (line[j] == '\"' && psr_has_char(END_DQUOTE, line + j) == 0))
				return (-1);
		}
		else if (psr_is_char(CHR_SPACE, line + j)
			&& (psr_has_char(END_LINE, line + j)
				|| psr_has_char(END_SPECIAL, line + j)))
			break ;
		j++;
	}
	cmd = ft_substr(line, 0, j - 0);
	if (cmd == NULL)
		return (-1);
	*i += j;
	printf("cmd = '%s'\n", cmd); // REMOVE
	free(cmd);
	
	// the argument part? 
	
	return (0);
}

int	psr_extract_path(char *name, int *i, char *line)
{
	char	*path;
	int		j;
	int		k;

	if (i == NULL)
		return (-1);
	j = 0;
	while (line[j] && psr_is_char(CHR_SPACE, line + j) == 1)
		j++;
	k = j;
	while (line[k] && psr_is_char(CHR_SPACE, line + k) == 0
		&& psr_is_char(CHR_SPECIAL, line + k) == 0)
		k++;
	path = ft_substr(line, j, (k - j));
	if (path == NULL)
		return (-1);
	*i += k;
	printf("%s = '%s'\n", name, path); // REMOVE
	free(path);
	return (0);
}


// (27/12) What to do?

// - Start implementing exit status management. The exit status 
//	of ^C should be 130.

// Why do you extract from the line commands, arguments and 
// file paths information?

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
	int		i;
	char	except;

	i = 0;
	except = 0;
	while (line[i])
	{
		// if line[i] == "\"", something should happen, but what?
		
		// if (line[i] == '"' || line[i] == '\'')
		// {
		// 	i += 1;
		// 	except = line[i];
		// 	// Manage matching " character or absence of matching character.
		// }

		if (psr_is_char(CHR_SPACE, line + i) == 1)
			i += 1;
		else if (psr_is_char(CHR_SPECIAL, line + i) == 0
			&& psr_extract_cmd(&i, line + i) >= 0)
			i += 0;
		else if (psr_is_char(CHR_RDIN, line + i) == 1
			&& psr_extract_path("inpath", &i, line + i + 1) >= 0)
			i += 1;
		else if (psr_is_char(CHR_RDOUT, line + i) == 1
			&& psr_extract_path("outpath", &i, line + i + 1) >= 0)
			i += 1;
		else if (psr_is_char(CHR_HEREDOC, line + i) == 1
			&& psr_extract_path("here", &i, line + i + 2) >= 0)
			i += 2;
		else if (psr_is_char(CHR_APPEND, line + i) == 1
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