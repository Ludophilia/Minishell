/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:31:20 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/13 15:17:50 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lex_is_op(int c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	lex_is_sep(int c)
{
	return (c == ' ');
}

int	lex_is_quote(int c)
{
	return (c == '\'' | c == '\"');
}

int	lex_print_tokens(t_tok *tokens)
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
