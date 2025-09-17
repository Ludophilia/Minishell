/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/17 23:46:48 by jegerman         ###   ########.fr       */
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
	// ft_bzero(core->cmds, CMD_MAX * sizeof(t_cmd));
	core->cmd_nbr = 0;
	cmd = core->cmds + core->cmd_nbr;
	tok = toks;

	// 17/09: Need to free those words...
	
	// But where are they?

	// 	(t_cmd) core->cmds
	// 		- (t_cmd) cmd
	//			- (char **) .argv <- malloc'd
	//				- (char *) arg -> malloc'd
	//			- (t_red *) .*reds
	//				- (char *) word <- malloc'd
	
	while (tok->type != TOK_EOL)
	{
		// Where are the routines for destroying data?


		if (psr_add_cmd(tok, cmd) == -1 || psr_add_reds(tok, cmd) == -1)
			return (-1);
		while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
			tok++;
		if (tok->type == TOK_PIPE)
			cmd = (++tok, core->cmds + ++core->cmd_nbr);
		// Nothing else to add once the structures are full?
	}
	return (0);
}
