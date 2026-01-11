/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/11 14:49:56 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	lex_print_tokens(t_tok *tokens)
// {
// 	t_tok	*token;
// 	int		i;

// 	i = -1;
// 	while ((token = tokens + ++i)->type != TOK_EOL)
// 	{
// 		if (token->type == TOK_WORD)
// 		{
// 			write(1, "\tTOK_WORD: ", 11);
// 			write(1, token->start, token->len);
// 			write(1, "\n", 1);
// 		}
// 		else if (token->type == TOK_AND)
// 			printf("\tTOK_AND\n");
// 		else if (token->type == TOK_OR)
// 			printf("\tTOK_OR\n");
// 		else if (token->type == TOK_SUBO)
// 			printf("\tTOK_SUBO\n");
// 		else if (token->type == TOK_SUBC)
// 			printf("\tTOK_SUBC\n");
// 		else if (token->type == TOK_IRED)
// 			printf("\tTOK_IRED\n");
// 		else if (token->type == TOK_IRED_HD)
// 			printf("\tTOK_IRED_HD\n");
// 		else if (token->type == TOK_ORED)
// 			printf("\tTOK_ORED\n");
// 		else if (token->type == TOK_ORED_AP)
// 			printf("\tTOK_ORED_AP\n");
// 		else if (token->type == TOK_PIPE)
// 			printf("\tTOK_PIPE\n");
// 	}
// 	return (0);
// }

static int	print_nodes_prefix_rev(int level, int left, int right, t_astn *root)
{
	char	**toks;
	char	**types;
	t_cmd	*cmd;
	t_tok	*tok;
	t_red	*reds;

	toks = (char *[]){"none", "word", "pipe", "ired", "hdoc",
		"ored", "ored_apn", "and", "or", "(", ")", 0};
	types = (char *[]){"none", "ao", "pipe", "sub", "cmd", 0};
	if (root == NULL && printf("Nothing to print\n"))
		return (0);
	fflush(NULL);
	for (int i = 0; i < level; ++i)
		printf("\t");
	if (left && right)
		printf("[C]");
	else
		printf(left? "[L]": "[R]");
	if (root->type == 1)
		printf("[%i] type: %s, op: %s\n", level, types[root->type], toks[root->op]);
	else if (root->type == 2 || (root->type == 3 && !root->content))
		printf("[%i] type: %s\n", level, types[root->type]);
	else if (root->type == 3 && root->content)
	{
		cmd = root->content;
		printf("[%i] type: %s, red: ", level, types[root->type]);
		fflush(NULL);
		reds = cmd->reds;
		tok = reds->word_tk;
		write(1, tok->start, tok->len);
		write(1, "\n", 1);
	}
	else if (root->type == 4)
	{
		printf("[%i] type: %s, start: ", level, types[root->type]);
		cmd = root->content;
		tok = *cmd->argv_tk;
		if (tok == NULL)
			printf("%p\n", tok);
		else
		{
			fflush(NULL);
			write(1, tok->start, tok->len);
			write(1, "\n", 1);
		}
	}
	if (root->right)
		print_nodes_prefix_rev(level + 1, 0, 1, root->right);
	if (root->left)
		print_nodes_prefix_rev(level + 1, 1, 0, root->left);
	return (0);
}

int	psr_parse_line(char *line, t_core *core)
{
	t_tok	*toks;

	toks = core->toks;
	if (lex_tokenize_line(line, toks)
		|| lex_error_check(toks, core) == -1)
		return (-2);
	if (psr_build_ast(toks, core) == -1)
		return (-1);
	print_nodes_prefix_rev(0, 1, 1, core->ast); // 19/12 - Will be removed...
	core->flags |= FLG_AST;
	return (0);
}
