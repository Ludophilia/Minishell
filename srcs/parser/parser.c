/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/11 11:24:47 by jegerman         ###   ########.fr       */
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

// 11/10 - RIP...
int	psr_parse_line(char *line, t_core *core)
{
	t_tok	toks[TOK_MAX];
	t_tok	*tok;
	t_cmd	*cmd;

	if (lex_tokenize_line(line, toks) || psr_error_check(toks, core) == -1)
		return (-2);

	lex_print_tokens(toks);
	(void)tok;
	(void)cmd;

	// 11/10 - First improve the lexer...
	// cmd = core->cmds + core->cmd_pmax;
	// tok = toks;
	// while (tok->type != TOK_EOL)
	// {
	// 	if (psr_add_cmd(tok, cmd, core) == -1
	// 		|| psr_add_reds(tok, cmd, core) == -1)
	// 	{
	// 		core->flags |= FLG_ALL;
	// 		return (-1);
	// 	}
	// 	while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
	// 		tok++;
	// 	if (tok->type == TOK_PIPE)
	// 		cmd = (++tok, core->cmds + ++core->cmd_pmax);
	// }


	core->flags |= FLG_ALL;
	return (0);
}
