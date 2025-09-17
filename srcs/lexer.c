/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 19:46:25 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/17 20:17:56 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lex_emit_token(t_tokt type, char *line, int len, t_tok *tok)
{
	tok->type = type;
	tok->start = line;
	tok->len = len;
	return (len);
}

static int	lex_emit_wtoken(char *line, t_tok *tok)
{
	int		quoted;
	int		j;

	quoted = 0;
	j = -1;
	while (line[++j])
	{
		if (!quoted && lex_is_quote(line[j]))
			quoted = line[j];
		else if (quoted && line[j] == quoted)
			quoted = 0;
		if (!quoted && (lex_is_sep(line[j]) || lex_is_op(line[j])))
			break ;
	}
	lex_emit_token(TOK_WORD, line, j, tok);
	return (j);
}

static int	lex_emit_optoken(char *line, t_tok *tok)
{
	int		j;

	j = 0;
	if (line[j] == '|')
		j += lex_emit_token(TOK_PIPE, line, 1, tok);
	else if (line[j] == '<' && line[j + 1] != '<')
		j += lex_emit_token(TOK_IRED, line, 1, tok);
	else if (line[j] == '<' && line[j + 1] == '<')
		j += lex_emit_token(TOK_IRED_HD, line, 2, tok);
	else if (line[j] == '>' && line[j + 1] != '>')
		j += lex_emit_token(TOK_ORED, line, 1, tok);
	else if (line[j] == '>' && line[j + 1] == '>')
		j += lex_emit_token(TOK_ORED_AP, line, 2, tok);
	return (j);
}

int	lex_tokenize_line(char *line, t_tok *toks)
{
	int		tpos;
	int		i;

	i = 0;
	tpos = 0;
	while (line[i])
	{
		if (!lex_is_sep(line[i]) && !lex_is_op(line[i]))
			i += lex_emit_wtoken(line + i, toks + tpos++);
		else if (lex_is_op(line[i]))
			i += lex_emit_optoken(line + i, toks + tpos++);
		else
			i++;
	}
	lex_emit_token(TOK_EOL, line + i, 1, toks + tpos);
	return (0);
}
