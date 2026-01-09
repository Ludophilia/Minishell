/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:57:18 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/09 19:22:32 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #############################################################################

// static int	psr_consume_wtok(t_cnt *c, t_tok *tok, t_cmd *cmd, t_core *core)
// {
// 	char	*word;

// 	word = exp_create_word(tok, TOK_WORD, core);
// 	if (word == NULL)
// 		return (c->f++, -1);
// 	// 5/01: Why this??? Why is "" filtered?
// 	// if (*word != 0)
// 	cmd->argv[cmd->argc++] = word;
// 	// else
// 	// 	free(word);
// 	c->i++;
// 	return (0);
// }

// static int	psr_consume_rtok(t_cnt *c, t_tok *tok, t_cmd *cmd, t_core *core)
// {
// 	t_red	*red;

// 	red = (cmd->reds + cmd->red_pmax);
// 	red->type = tok->type;
// 	red->word = exp_create_word(tok + 1, red->type, core);
// 	if (red->word == NULL)
// 		return (c->f++, -1);
// 	cmd->red_pmax++;
// 	c->i += 2;
// 	return (0);
// }


static int	psr_count_args(t_tok *tok)
{
	int	size;

	size = 0;
	while (tok->type == TOK_WORD || lex_isred(tok))
	{
		if (tok->type != TOK_WORD)
			tok += 2;
		else
			(void)(size++, tok++);
	}
	return (size);
}

static int	psr_store_rtok(t_cnt *c, t_tok *tok, t_cmd *cmd)
{
	t_red	*red;

	red = (cmd->reds + cmd->red_pmax);
	red->type = tok->type;
	red->word_tk = (tok + 1);
	cmd->red_pmax++;
	c->i += 2;
	return (0);
}

static int	psr_store_wtok(t_cnt *c, t_tok *tok, t_cmd *cmd)
{
	cmd->argv_tk[cmd->argc++] = tok;
	c->i++;
	return (0);
}

static int	psr_prefill_cmd(t_cnt *c, t_tok *toks, t_core *core, t_cmd *cmd)
{
	t_tok	*tok;
	int		len;
	int		is_word;

	len = psr_count_args(toks + c->i);
	cmd->argv_tk = ft_calloc(len + 1, sizeof(t_tok *));
	if (cmd->argv_tk == NULL)
		return (c->f++, -1);
	tok = toks + c->i;
	while (tok->type == TOK_WORD || lex_isred(tok))
	{
		is_word = (tok->type == TOK_WORD);
		if ((is_word && psr_store_wtok(c, tok, cmd) == -1)
			|| (!is_word && psr_store_rtok(c, tok, cmd) == -1))
			return (-1);
		tok = toks + c->i;
	}
	return (0);
}

int	psr_rdp_scmd(t_cnt *c, t_tok *toks, t_core *core, t_astn *node)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (cmd == NULL)
		return (c->f++, -1);
	if (psr_prefill_cmd(c, toks, core, cmd) == -1)
	{
		free(cmd);
		return (-1);
	}
	core->cmds++;
	node->content = cmd;
	return (0);
}
