/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rdp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:20:26 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/13 20:58:26 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*psr_rdp_smpcmd(t_cnt *c, t_tok *toks, t_core *core)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (cmd == NULL)
		return (c->f++, NULL);
	if (psr_fill_cmd(c, toks, core, cmd) == -1)
	{
		free(cmd);
		return (c->f++, NULL);
	}
	return (cmd);
}

static t_cmdn	*psr_rdp_cmd(t_cnt *c, t_tok *toks, t_core *core)
{
	t_cmdn	*new;

	new = psr_new_cmdn(AST_CMD);
	if (new == NULL)
		return (c->f++, NULL);
	if (toks[c->i].type == TOK_SUBO)
	{
		c->i++;
		new->type = AST_SUB;
		new->cont = psr_rdp_line(c, toks, core);
		if (toks[c->i].type == TOK_SUBC)
			c->i++;
	}
	else if (toks[c->i].type == TOK_WORD || psr_isred((toks + c->i)))
		new->cont = psr_rdp_smpcmd(c, toks, core);
	return (new);
}

static t_astn	*psr_rdp_pipe(t_cnt *c, t_tok *toks, t_core *core)
{
	t_astn	*new;
	t_astn	*old;

	new = psr_new_astn(AST_PI);
	if (new == NULL)
		return (c->f++, NULL);
	new->left = psr_rdp_cmd(c, toks, core);
	if (toks[c->i].type == TOK_PIPE)
		new->op = toks[c->i++].type;
	new->right = psr_rdp_cmd(c, toks, core);

	while (toks[c->i].type == TOK_PIPE)
	{
		old = new;
		new = psr_new_astn(AST_PI);
		if (new == NULL)
			return (c->f++, old);

		new->left = old;
		new->op = toks[c->i++].type;
		new->right = psr_rdp_cmd(c, toks, core);
	}
	return (new);
}

/* 13/12 - Correction suggestion... ChatGPT ver.

t_astn *psr_rdp_andor(...)
{
    t_astn *node = psr_rdp_pipe(c, toks, core);

    while (toks[c->i].type == TOK_AND || toks[c->i].type == TOK_OR)
    {
        t_astn *new = psr_new_astn(AST_AO);
        new->left = node;
        new->op = toks[c->i++].type;
        new->right = psr_rdp_pipe(c, toks, core);
        node = new;
    }
    return node;
}
*/


static t_astn	*psr_rdp_andor(t_cnt *c, t_tok *toks, t_core *core)
{
	t_astn	*new;
	t_astn	*old;

	new = psr_new_astn(AST_AO);
	if (new == NULL)
		return (c->f++, NULL);

	new->left = psr_rdp_pipe(c, toks, core);


	if (toks[c->i].type == TOK_AND || toks[c->i].type == TOK_OR)
		new->op = toks[c->i++].type;
	new->right = psr_rdp_pipe(c, toks, core);
	while (toks[c->i].type == TOK_AND || toks[c->i].type == TOK_OR)
	{
		old = new;
		new = psr_new_astn(AST_AO);
		if (new == NULL)
			return (c->f++, old);
		new->left = old;
		new->op = toks[c->i++].type;
		new->right = psr_rdp_pipe(c, toks, core);
	}
	return (new);
}

// 13/12 - Ohohoh... This is not a Syntax tree, it's a PARSE tree.
// Non terminals are not abstracted away, so it doesn't exactly include
// the real tokens which makes it more expensive to run. 
t_astn	*psr_rdp_line(t_cnt *c, t_tok *toks, t_core *core)
{
	t_astn	*ao_node;

	if (toks->type == TOK_EOL)
		return (c->f++, NULL);
	ao_node = psr_rdp_andor(c, toks, core);
	return (ao_node);
}


	// Do I need a guard here?
	//	- OK for creating a PIPE node.
	// 	- [OK] TOK_WORD, TOK_SUBO, TOK_RED, TOK_PIPE,
	//	- [KO] TOK_EOL, TOK_SUBC, TOK_AND, TOK_OR