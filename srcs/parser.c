/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/06 18:59:43 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lex_print_tokens(t_tok *tokens)
{
	int		i;
	t_tok	*token;

	i = -1;
	while ((token = tokens + ++i)->type != TOK_EOL)
	{
		if (token->type == TOK_WORD)
		{
			write(1, "\tTOK_WORD: ", 11);
			write(1, token->start, token->len);
			write(1, "\n", 1);
		}
		else if (token->type == TOK_IRED)
			printf("\tTOK_IRED\n");
		else if (token->type == TOK_IRED_HD)
			printf("\tTOK_IRED_HD\n");
		else if (token->type == TOK_ORED)
			printf("\tTOK_ORED\n");
		else if (token->type == TOK_ORED_AP)
			printf("\tTOK_ORED_AP\n");
		else if (token->type == TOK_PIPE)
			printf("\tTOK_PIPE\n");
	}
	return (0);
}

// Errors:
//	- WORD.
//		- Unmatched quote.
//	- 

int	psr_op_check(t_tok *token, int pos)
{
	t_tokt	*ops_tks;
	int		j;

	ops_tks = (t_tokt[6]){TOK_PIPE, TOK_IRED, TOK_IRED_HD, TOK_ORED,
		TOK_ORED_AP, TOK_EOL};
	j = -1;	
	while (ops_tks[++j])
	{
		if (token->type == ops_tks[j] && token[1].type == TOK_EOL)
			return (-1);
		if (ops_tks[j] == TOK_PIPE
			&& token->type == ops_tks[j]
			&& token[1].type == TOK_PIPE);
	}
	return (0);
}

int	psr_error_check(t_tok *tokens)
{
	char	**tok_chr;
	t_tok	*token;
	int		i;

	tok_chr = (char *[]){"|", "<", "<<", ">", ">>"};
	token = tokens + i;
	i = 0;
	while (token->type)
	{
		if ()

		token = tokens + ++i;
	}
	return (0);
}

int	psr_parse_line(char *line, t_core *core)
{
	t_tok	tokens[TOK_MAX];

	(void)core;
	if (lex_tokenize_line(line, tokens) ||
		psr_error_check(tokens) == -1)
	{
		ft_eprintf("Something went wrongue.\n");
		return (-1);
	}
	lex_print_tokens(tokens);
	return (0);
}
