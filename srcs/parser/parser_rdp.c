/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rdp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:20:26 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/12 19:35:13 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*psr_rdp_scmd(t_cnt *c, t_tok *toks, t_core *core)
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

static t_cmdn	*psr_rdp_cmdnode(t_cnt *c, t_tok *toks, t_core *core)
{
	t_cmdn	*cmdn;

	cmdn = ft_calloc(1, sizeof(t_cmdn));
	if (cmdn == NULL)
		return (c->f++, NULL);
	if (toks[c->i].type == TOK_SUBO)
	{
		c->i++;
		cmdn->sub = psr_rdp_line(c, toks, core);
		if (toks[c->i].type == TOK_SUBC)
			c->i++;
	}
	else if (toks[c->i].type == TOK_WORD || psr_isred((toks + c->i)))
		cmdn->cmd = psr_rdp_scmd(c, toks, core);
	return (cmdn);
}

static t_pipn	*psr_rdp_pipeline(t_cnt *c, t_tok *toks, t_core *core)
{
	t_pipn	*pi_node;

	pi_node = ft_calloc(1, sizeof(t_pipn));
	if (pi_node == NULL)
		return (c->f++, NULL);
	if (toks[c->i].type != TOK_EOL && psr_isop(toks + c->i) == false)
		pi_node->left = psr_rdp_cmdnode(c, toks, core);
	if (toks[c->i].type == TOK_PIPE)
		pi_node->op = toks[c->i++].type;
	if (toks[c->i].type != TOK_EOL && psr_isop(toks + c->i) == false) // && echo b ; && (echo b)
		pi_node->right = psr_rdp_cmdnode(c, toks, core);
	return (pi_node);
}

// 13/12 = UP UP UP

// 12/12 - YES! YES! YES!! We're getting somewhere... 
// That seems correct this time.
static t_astn	*psr_rdp_andor(t_cnt *c, t_tok *toks, t_core *core)
{
	t_astn	*new;
	t_astn	*old;

	new = ft_calloc(1, sizeof(t_astn));
	if (new == NULL)
		return (c->f++, NULL);

	// Do I need a guard here?
	new->left = psr_rdp_pipeline(c, toks, core);
	if (toks[c->i].type == TOK_AND || toks[c->i].type == TOK_OR)
		new->op = toks[c->i++].type;
	// Do I need a guard here?
	new->right = psr_rdp_pipeline(c, toks, core);
	while (toks[c->i].type == TOK_AND || toks[c->i].type == TOK_OR)
	{
		old = new;
		new = ft_calloc(1, sizeof(t_astn));
		if (new == NULL)
			return (c->f++, old);
		new->left = old;
		new->op = toks[c->i++].type;
		// Do I need a guard here?
		new->right = psr_rdp_pipeline(c, toks, core);
	}
	return (new);
}

t_astn	*psr_rdp_line(t_cnt *c, t_tok *toks, t_core *core)
{
	t_astn	*ao_node;

	if (toks->type == TOK_EOL) // ???
		return (c->f++, NULL);
	ao_node = psr_rdp_andor(c, toks, core);
	return (ao_node);
}

// ######################################################################

// 9/12 - Do we get the AST from what I wrote?
// lv-a0: echo a [9/12 - Seems OK.]
// lv-a1: echo a && echo b [9/12 - Seems OK.]
// lv-a2: echo a && (echo b || echo c) [10/12 - Seems OK.]

// lv-b0: echo a | tee [10/12 - Seems OK.]
// lv-b1: echo a | tee | cat

// More tests coming... 10/12.