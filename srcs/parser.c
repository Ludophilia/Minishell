/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/20 00:57:30 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_parse_line(char *line, t_core *core)
{
	t_tok	toks[TOK_MAX];
	t_tok	*tok;
	t_cmd	*cmd;

	if (lex_tokenize_line(line, toks) || psr_error_check(toks) == -1)
		return (-1);
	cmd = core->cmds + core->cmd_pos;
	tok = toks;
	while (tok->type != TOK_EOL)
	{
		if (psr_add_cmd(tok, cmd) == -1 || psr_add_reds(tok, cmd) == -1)
		{
			utl_cleanup(FLG_CMDS, core);
			return (-1);
		}
		while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
			tok++;
		if (tok->type == TOK_PIPE)
			cmd = (++tok, core->cmds + ++core->cmd_pos);
		// Nothing else to add once the structures are full?
	}
	core->flags |= (FLG_CMDS);
	return (0);
}
