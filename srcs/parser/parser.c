/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/07 22:26:33 by jegerman         ###   ########.fr       */
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

// 2/12 - OK, here we are...
// So how to change that logic so that we can integrate it into
// the AST?

// 4/12 - What should be done?
// == Fill the command node with the correct information pulled from TOKS
// == == Fill the t_cmd (argv, reds)...
// == == 
t_cmd	*psr_rdp_cmd(t_cnt *c, t_tok *toks, t_core *core)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (cmd == NULL)
		return (c->f++, NULL);
	if (psr_add_cmd(c, toks, cmd, core) == -1)
	{
		free(cmd);
		return (c->f++, NULL);
	}
	return (cmd);
}

t_cmdn	*psr_rdp_cmdnode(t_cnt *c, t_tok *toks, t_core *core)
{
	t_cmdn	*cmdn;

	cmdn = ft_calloc(1, sizeof(t_cmdn));
	if (cmdn == NULL)
		return (c->f++, NULL);
	if (toks[c->i].type == TOK_SUBO)
	{
		c->i++;
		cmdn->sub = psr_rdp_line(c, toks, core);
		if (toks[c->i].type == TOK_SUBC) // Will we really get to TOK_SUBC "naturally"? // DON'T KNOW, we will see...
			c->i++;
	}
	else if (toks[c->i].type != TOK_EOL)
		cmdn->cmd = psr_rdp_cmd(c, toks, core);
	return (cmdn);
}

t_pipn	*psr_rdp_pipeline(t_cnt *c, t_tok *toks, t_core *core)
{
	t_pipn	*pi_node;

	pi_node = ft_calloc(1, sizeof(t_pipn));
	if (pi_node == NULL)
		return (c->f++, NULL);
	if (toks[c->i].type != TOK_EOL)
		pi_node->left = psr_rdp_cmdnode(c, toks, core);
	if (toks[c->i].type != TOK_EOL)
		pi_node->right = psr_rdp_cmdnode(c, toks, core);
	return (pi_node);
}

t_logn	*psr_rdp_andor(t_cnt *c, t_tok *toks, t_core *core)
{
	t_logn	*aon;

	aon = ft_calloc(1, sizeof(t_logn));
	if (aon == NULL)
		return (c->f++, NULL);
	if (toks[c->i].type != TOK_EOL) // if EOL -> NULL
		aon->left = psr_rdp_pipeline(c, toks, core);
	// We're supposing that the right OPTOK if exists, will come once pipeline
	// is processed.
	if (toks[c->i].type == TOK_AND || toks[c->i].type == TOK_OR)
		aon->op = toks[c->i++].type;
	if (toks[c->i].type != TOK_EOL) // if EOL -> NULL
		aon->right = psr_rdp_pipeline(c, toks, core);
	return (aon);
}

// 29/11 - What should be done?
// A line is replaced by an and/or. So return the and/or node.
t_logn	*psr_rdp_line(t_cnt *c, t_tok *toks, t_core *core)
{
	t_logn	*ao_node;

	if (toks->type == TOK_EOL)
		return (c->f++, NULL);
	ao_node = psr_rdp_andor(c, toks, core);
	return (ao_node);
}

// 29/11 - Here we are pals.
int	psr_build_ast(t_tok *toks, t_core *core)
{
	t_cnt	c;

	// c.i = 0;
	// c.f = 0;
	c = (t_cnt){.f = 0, .i = 0}; // May not work work work/

	// 4/12 - Keep going from there.
	core->ast = psr_rdp_line(&c, toks, core);
	
	// 6/12 - Ahahahaha. PLEASE implement NOT_IMPL_DESTROY_AST asap
	if (core->ast == NULL || (c.f > 0 && NOT_IMPL_DESTROY_AST(core)))
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

	// core->flags |= FLG_ALL;

	// How do I create it?

	// - 0. Create the AST
	// - -- [x] 1. Identity and add the different possibles nodes in minishell.h

	// - [o] 2. Write the Recursive Descent Parser logic.
	//	- Those multiple functions... one for each non terminal, starting
	//   from top level, where the AST is building itself bottom-up and where
	//   the input should be visible from the leaves of the AST while using

	// - [ ] 3. Find a way to test it.

	core->flags |= FLG_ALL;
	return (0);
}
