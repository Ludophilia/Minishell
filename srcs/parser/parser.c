/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/09 17:44:10 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lex_print_tokens(t_tok *tokens)
{
	t_tok	*token;
	int		i;

	i = -1;
	while ((token = tokens + ++i)->type != TOK_EOL)
	{
		if (token->type == TOK_WORD)
		{
			write(1, "\tTOK_WORD: ", 11);
			write(1, token->start, token->len);
			write(1, "\n", 1);
		}
		else if (token->type == TOK_AND)
			printf("\tTOK_AND\n");
		else if (token->type == TOK_OR)
			printf("\tTOK_OR\n");
		else if (token->type == TOK_SUBO)
			printf("\tTOK_SUBO\n");
		else if (token->type == TOK_SUBC)
			printf("\tTOK_SUBC\n");
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

// 9/12 == Implement destroy AST.

int	psr_build_ast(t_tok *toks, t_core *core)
{
	t_cnt	c;

	c.f = 0;
	c.i = 0;
	core->ast = psr_rdp_line(&c, toks, core);
	if (core->ast == NULL || (c.f > 0 && NOT_IMPL_DESTROY_AST(core)))
		return (-1);
	return (0);
}

int	psr_parse_line(char *line, t_core *core)
{
	t_tok	toks[TOK_MAX];

	if (lex_tokenize_line(line, toks) || psr_error_check(toks, core) == -1)
		return (-2);
	// lex_print_tokens(toks);
	psr_build_ast(toks, core);
	// 9/12 - Find a way to test the AST.
	core->flags |= FLG_ALL;
	return (0);
}
