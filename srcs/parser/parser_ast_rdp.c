/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_rdp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:20:26 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/27 19:35:14 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_rdp_sub(t_cnt *c, t_tok *toks, t_core *core, t_astn *node)
{
	c->i++;
	node->type = AST_SUB;
	node->left = psr_rdp_line(c, toks, core);
	if (toks[c->i].type != TOK_SUBC)
		return (c->f++, -1);
	c->i++;
	if (psr_isred(toks + c->i) == false)
		return (0);
	if (psr_rdp_scmd(c, toks, core, node) == -1)
		return (-1);
	return (0);
}

static t_astn	*psr_rdp_cmd(t_cnt *c, t_tok *toks, t_core *core)
{
	t_astn	*node;

	if (toks[c->i].type != TOK_SUBO && toks[c->i].type != TOK_WORD
		&& psr_isred(toks + c->i) == false)
		return (NULL);
	node = psr_new_astn(AST_CMD);
	if (node == NULL)
		return (c->f++, NULL);
	if ((toks[c->i].type == TOK_WORD || psr_isred(toks + c->i))
		&& psr_rdp_scmd(c, toks, core, node) == -1)
		return (node);
	else if (toks[c->i].type == TOK_SUBO
		&& psr_rdp_sub(c, toks, core, node) == -1)
		return (node);
	return (node);
}

static t_astn	*psr_rdp_pipeline(t_cnt *c, t_tok *toks, t_core *core)
{
	t_astn	*node;
	t_astn	*new;

	node = psr_rdp_cmd(c, toks, core);
	if (node == NULL)
		return (NULL);
	while (toks[c->i].type == TOK_PIPE)
	{
		new = psr_new_astn(AST_PI);
		if (new == NULL)
			return (c->f++, node);
		new->left = node;
		new->op = toks[c->i++].type;
		new->right = psr_rdp_cmd(c, toks, core);
		if (new->right == NULL)
			return (new);
		node = new;
	}
	return (node);
}

static t_astn	*psr_rdp_andor(t_cnt *c, t_tok *toks, t_core *core)
{
	t_astn	*node;
	t_astn	*new;

	node = psr_rdp_pipeline(c, toks, core);
	if (node == NULL)
		return (NULL);
	while (toks[c->i].type == TOK_AND || toks[c->i].type == TOK_OR)
	{
		new = psr_new_astn(AST_AO);
		if (new == NULL)
			return (c->f++, node);
		new->left = node;
		new->op = toks[c->i++].type;
		new->right = psr_rdp_pipeline(c, toks, core);
		if (new->right == NULL)
			return (new);
		node = new;
	}
	return (node);
}

t_astn	*psr_rdp_line(t_cnt *c, t_tok *toks, t_core *core)
{
	t_astn	*ao_node;

	if (toks[c->i].type == TOK_EOL)
		return (c->f++, NULL);
	ao_node = psr_rdp_andor(c, toks, core);
	if (ao_node == NULL)
		return (NULL);
	return (ao_node);
}
