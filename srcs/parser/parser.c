/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/09 00:40:20 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int psr_parse_line(char *line, t_core *core)
{
    t_tok toks[TOK_MAX];
    t_tok *tok;
    t_cmd *cmd;

    if (lex_tokenize_line(line, toks) || psr_error_check(toks) == -1)
        return (-2);
    cmd = core->cmds + core->cmd_pmax;
    tok = toks;
    while (tok->type != TOK_EOL)
    {
        if (psr_add_cmd(tok, cmd) == -1 || psr_add_reds(tok, cmd) == -1)
        {
            core->flags |= (FLG_ALL);
            return (-1);
        }
        while (tok->type != TOK_PIPE && tok->type != TOK_EOL)
            tok++;
        if (tok->type == TOK_PIPE)
            cmd = (++tok, core->cmds + ++core->cmd_pmax);
    }
    core->flags |= (FLG_ALL);
    return (0);
}
