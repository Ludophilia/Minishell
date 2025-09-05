/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 19:46:25 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/05 18:32:10 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lex_emit_token(t_tokt type, char *line, int len, t_tok *token)
{
	token->type = type;
	token->start = line;
	token->len = len;
	return (len);
}

static int	lex_emit_wtoken(char *line, t_tok *token)
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
	lex_emit_token(TOK_WORD, line, j, token);
	return (j);
}

static int	lex_emit_optoken(char *line, t_tok *token)
{
	int		j;

	j = 0;
	if (line[j] == '|')
		j += lex_emit_token(TOK_PIPE, line, 1, token);
	else if (line[j] == '<' && line[j + 1] != '<')
		j += lex_emit_token(TOK_IRED, line, 1, token);
	else if (line[j] == '<' && line[j + 1] == '<')
		j += lex_emit_token(TOK_IRED_HD, line, 2, token);
	else if (line[j] == '>' && line[j + 1] != '>')
		j += lex_emit_token(TOK_ORED, line, 1, token);
	else if (line[j] == '>' && line[j + 1] == '>')
		j += lex_emit_token(TOK_ORED_AP, line, 2, token);
	return (j);
}

static int	lex_print_tokens(t_tok *tokens)
{
	int		i;
	t_tok	*token;

	i = -1;
	while ((token = tokens + ++i)->type != TOK_EOL)
	{
		if (token->type == TOK_WORD)
		{
			write(1, "\tTOK_WORD: ", 11);
			write(1, token->start, token->len);
			write(1, "\n", 1);
		}
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

int	lex_tokenize_line(char *line, t_tok *tokens)
{
	int		tpos;
	int		i;

	i = 0;
	tpos = 0;
	while (line[i])
	{
		if (!lex_is_sep(line[i]) && !lex_is_op(line[i]))
			i += lex_emit_wtoken(line + i, tokens + tpos++);
		else if (lex_is_op(line[i]))
			i += lex_emit_optoken(line + i, tokens + tpos++);
		else
			i++;
	}
	lex_emit_token(TOK_EOL, line + i, 1, tokens + tpos);
	lex_print_tokens(tokens); // Remove
	return (0);
}
