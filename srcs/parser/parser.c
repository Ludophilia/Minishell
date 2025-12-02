/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/02 21:42:11 by jegerman         ###   ########.fr       */
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

// ============================================

// 29/11 - Let's try to write a recursive decent parser.
// == LL(1), TOP DOWN, one procedure per non-terminal

// ============================================

// 29/11 - What should be done...

// This is where the real work begin.
// ITERATE on TOKENS, LL(1) style

// WORD WORD AND WORD WORD (echo a && echo b)

// ============================================

t_cmd	*psr_rdp_cmd(int *i, t_tok *toks)
{
	t_tok	*tok;
	t_cmd	*cmd;

	// 2/12 - OK, here we are...
	// So how to change that logic so that we can integrate it into
	// the AST?

	cmd = core->cmds + core->cmd_pmax;
	tok = toks;
	while (tok->type != TOK_EOL)
	{
		if (psr_add_cmd(tok, cmd, core) == -1
			|| psr_add_reds(tok, cmd, core) == -1)
		{
			core->flags |= FLG_ALL;
			return (-1);
		}
		while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
			tok++;
		if (tok->type == TOK_PIPE)
			cmd = (++tok, core->cmds + ++core->cmd_pmax);
	}
	return ();
}


t_cmdn	*psr_rdp_cmdn(int *i, t_tok *toks)
{
	t_cmdn	*cmdn;

	cmdn = ft_calloc(1, sizeof(t_cmdn));
	if (cmdn == NULL)
		return (NULL);
	if (toks[*i].type == TOK_SUBO)
	{
		(*i)++;
		cmdn->is_sub = 1;
		cmdn->sub = psr_rdp_line(i, toks);
		if (toks[*i].type == TOK_SUBC)
			(*i)++;
	}
	else if (toks[*i].type != TOK_EOL)
		cmdn->cmd = ; // >???
	return (cmdn);
}

t_pipn	*psr_rdp_pipeline(int *i, t_tok *toks)
{
	t_pipn	*pi_node;

	pi_node = ft_calloc(1, sizeof(t_pipn));
	if (pi_node == NULL)
		return (NULL);
	if (toks[*i].type != TOK_EOL)
		pi_node->left = psr_rdp_cmdn(&i, toks);
	if (toks[*i].type != TOK_EOL)
		pi_node->right = psr_rdp_cmdn(&i, toks);
	return (pi_node);
}

t_logn	*psr_rdp_andor(int *i, t_tok *toks)
{
	t_logn	*aon;

	aon = ft_calloc(1, sizeof(t_logn));
	if (aon == NULL)
		return (NULL);
	if (toks[*i].type != TOK_EOL)
		aon->left = psr_rdp_pipeline(i, toks);
	if (toks[*i].type == TOK_AND || toks[*i].type == TOK_OR)
		aon->op = toks[(*i)++].type;
	if (toks[*i].type != TOK_EOL)
		aon->right = psr_rdp_pipeline(i, toks);
	return (aon);
}

// 29/11 - What should be done?
// A line is replaced by an and/or. So return the and/or node.
t_logn	*psr_rdp_line(int *i, t_tok *toks)
{
	t_logn	*ao_node;

	if (toks->type == TOK_EOL);
		return (NULL);
	ao_node = psr_rdp_andor(i, toks);
	return (ao_node);
}

// 29/11 - Code so small that it could sent back to the calling function...
int	psr_build_ast(t_tok *toks, t_core *core)
{
	int	i;

	i = 0;
	core->ast = psr_rdp_line(&i, toks);
	if (core->ast == NULL)
		return (-1);
	return (0);
}

// ########################################################################

int	psr_parse_line(char *line, t_core *core)
{
	t_tok	toks[TOK_MAX];


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



	core->flags |= FLG_ALL;
	return (0);
}
