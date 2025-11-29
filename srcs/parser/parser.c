/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/11/29 19:49:25 by jegerman         ###   ########.fr       */
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

// Which nodes (look at the grammar node)
// = (AND / OR) node
// = PIPELINE node
// = COMMAND node
// = SUBSHELL node (?, COMMAND node)
// = REDIRECTION node (too much, that's command context...)

//RULES:

// - line				::= and_or
// - and_or				::= pipeline ( ('&&' | '||') pipeline )*
// - pipeline			::= command ( '|' command )*
// - command			::= simple_command | subshell
// - subshell			::= '(' line ')'
// - simple_command		::= word ( word | redirection )*
// - redirection		::= ('<' | '>' | '<<' | '>>') word
// - word				::= TOKEN

// 29/11 - Let's try to write a recursive decent parser.
// == LL(1), TOP DOWN, one procedure per non-terminal

t_logn	*psr_rdp_andor(t_tok *toks)
{
	// 29/11 - What should be done...

	// This is where the real work begin.
	// Iterate on token, LL(1) style

	// WORD WORD AND WORD WORD (echo a && echo b)

	// - We need to allocate memory (t_logn	*)

	// - 

	return ();
}

t_logn	*psr_rdp_line(t_tok *toks)
{
	// 29/11 - What should be done?
	// A line is replaced by an and/or. So return the and/or node. 
	return (psr_rdp_andor(toks));
}

int	psr_build_ast(t_tok *toks, t_core *core)
{
	// 29/11 - Code so small that it could sent back to the calling function...
	core->ast = psr_rdp_line(toks);
	if (core->ast == NULL)
		return (-1);
	return (0);
}


int	psr_parse_line(char *line, t_core *core)
{
	t_tok	toks[TOK_MAX];
	// t_tok	*tok;
	// t_cmd	*cmd;

	if (lex_tokenize_line(line, toks) || psr_error_check(toks, core) == -1)
		return (-2);

	lex_print_tokens(toks);



	// 20/11, 28/11 - OK. What should be done now?
		// - Parsing.
		// 		- Creating the AST from the tokens... 
		//		The meat of the subject bonus.

	psr_build_ast(toks, core);

	// How do I create it?

	// - 0. Create the AST
	// - -- [x] 1. Identity and add the different possibles nodes in minishell.h

	// - [o] 2. Write the Recursive Descent Parser logic.
	//	- Those multiple functions... one for each non terminal, starting
	//   from top level, where the AST is building itself bottom-up and where
	//   the input should be visible from the leaves of the AST while using
	//	 

	// - [ ] 3. Find a way to test it.
	// - 

	// (void)tok;
	// (void)cmd;

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
