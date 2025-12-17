/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/17 19:17:48 by jegerman         ###   ########.fr       */
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

int	print_nodes_prefix_rev(int level, int left, int right, t_astn *root)
{	
	char	**toks;
	char	**types;

	toks = (char *[]){"none", "word", "pipe", "ired", "hdoc",
		"ored", "ored_apn", "and", "or", "(", ")", 0};
	types = (char *[]){"none", "ao", "pipe", "sub", "cmd", 0};
	if (root == NULL)
		return (printf("Nothing to print\n"));
	if (root->type != 0)
	{
		for (int i = 0; i < level; ++i)
			printf("\t");
		if (left && right)
			printf("[C]");
		else
			printf(left? "[L]": "[R]");
		if (root->type == 1)
			printf("[%i] type: %s, op: %s\n", level, types[root->type],
				toks[root->op]);
		else if (root->type == 2 || root->type == 3)
			printf("[%i] type: %s\n", level, types[root->type]);
		if (root->type == 4)
			printf("[%i] type: %s, start: %s\n",
				level, types[root->type], *((t_cmd *)(root->left))->argv);
		if (root->type == AST_CMD)
			return (0);
	}
	if (root->right)
		print_nodes_prefix_rev(level + 1, 0, 1, root->right);
	if (root->left)
		print_nodes_prefix_rev(level + 1, 1, 0, root->left);
	return (0);
}

// 18/12 == Implement destroy AST. The comment below.
int	psr_build_ast(t_tok *toks, t_core *core)
{
	t_cnt	c;

	c.f = 0;
	c.i = 0;
	core->ast = psr_rdp_line(&c, toks, core);
	if (core->ast == NULL)
		return (-1);
	// if (core->ast == NULL || (c.f > 0 && NOT_IMPL_DESTROY_AST(core)))
	// 	return (-1);
	return (0);
}

int	psr_parse_line(char *line, t_core *core)
{
	t_tok	toks[TOK_MAX];

	if (lex_tokenize_line(line, toks) || psr_error_check(toks, core) == -1)
		return (-2);
	(void)lex_print_tokens;
	if (psr_build_ast(toks, core) == -1 && ft_eprintf("Error\n"))
		return (-1); // 17/12: -1?
	
	print_nodes_prefix_rev(0, 1, 1, core->ast);

	core->flags |= FLG_ALL;
	return (0);
}
