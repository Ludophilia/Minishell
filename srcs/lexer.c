/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 19:46:25 by jegerman          #+#    #+#             */
/*   Updated: 2025/08/31 22:00:16 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
## What should be done?

	- Emitting tokens as you scan a command line

## Why? What is the goal of that parser?

	- Maybe it helps for the parsing phase. Yes it does, so we can identify the
	commands and arguments and other operators (redirection, pipe) faster...

	- Identify the different parts of a command line?

	- Raising errors if the 

## Which tokens ?

	- Command (too early), arguments? (too early),

	|_op

	>_op, <_op, >>_op, <<_op, 

	- quoted_word? dq_word? 

## Conclusion?

	- Lexing will identify the PARTS

	- Parsing will MAKE SENSE of those PARTS:
		- ORDER / positionning of words give them special meaning
			- command is the first? WORD or the first WORD after some metachars like 
			'|' '&&' or some operations like redirection

*/

enum e_tokt
{
	TOK_;	
};

typedef struct s_tok
{
	enum e_tokt		type;
	char			*start;
	int				len; // or malloc
	struct s_tok	*next; // or array of tokens
}	t_tok;

int	lex_parse_line(char *line) // 31/08 - Temporary name
{
	int	i;
	//t_tok	tokens[128]; //

	i = 0;
	while (line[i])
		++i;
	return (0);
}
