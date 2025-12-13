/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/13 18:54:03 by jegerman         ###   ########.fr       */
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

// 13/12 == Lowe


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
	// ######################################################################

	// 9/12 - Do we get the AST from what I wrote?
	// lv-a0: echo a [9/12 - Seems OK.]
	// lv-a1: echo a && echo b [9/12 - Seems OK.]
	// lv-a2: echo a && (echo b || echo c) [10/12 - Seems OK.]

	// lv-b0: echo a | tee [10/12 - Seems OK.]
	// lv-b1: echo a | tee | cat

	// More tests coming... 10/12.
	core->flags |= FLG_ALL;
	return (0);
}
