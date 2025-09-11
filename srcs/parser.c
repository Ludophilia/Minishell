/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/11 15:35:45 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_optk_check(t_tok *token, int pos)
{
	char	**tok_chr;
	
	if (token->type == TOK_WORD)
		return (0);
	tok_chr = (char *[]){"|", "<", "<<", ">", ">>", 0};
	if (token[1].type == TOK_EOL 
		|| token[1].type == token->type
		|| (pos == 0 && token->type == TOK_PIPE))
	{
		ft_eprintf(ERR_SYNTAX, tok_chr[token->type - 2]);
		return (-1);
	}
	return (0);
}

int	ps_wtk_check(t_tok *token)
{
	int		i;
	int		quoted;

	if (token->type != TOK_WORD)
		return (0);
	i = -1;
	quoted = 0;
	while (++i < token->len)
	{
		if (!quoted && lex_is_quote(token->start[i]))
			quoted = token->start[i];
		else if (quoted && token->start[i] == quoted)
			quoted = 0;
	}
	if (quoted != 0 && ft_eprintf(ERR_SYNTAX, "end-of-line"))
		return (-1);
	return (0);
}

int	psr_error_check(t_tok *tokens)
{
	t_tok	*token;
	int		i;

	i = 0;
	token = tokens + i;
	while (token->type != TOK_EOL)
	{
		if (psr_optk_check(token, i) == -1
			|| ps_wtk_check(token) == -1)
			return (-1);
		token = tokens + ++i;
	}
	return (0);
}

// ============================================================================

// 11/09 - Is that what chaos look like? Sure, but you have to understand
// things before 


// 11/09 - fthat_malloc_expands_remove_quotes uses malloc and stuff...

char	*psr_create_word(char *start, int len)
{
	// Not implemented yet
	// 12/09
}

int	psr_add_red(t_red *reds, int *pos, t_tok *token, int *i)
{
	t_red	*red;

	red = reds + *pos;
	red->type = token->type;
	red->word = fthat_malloc_expands_remove_quotes(token[1].start, token[1].len);
	if (red->word == NULL)
		return (-1);
	*i += 1;
	*pos += 1;
	return (0);
}

// psr_damn_those_names
int	psr_cmd_count_words(t_tok *token)
{
	int	size;

	size = 0;
	while (token->type == TOK_WORD)
	{
		size++;
		token++;
	}
	return (size);
}

// 10/09
int	psr_add_cmd(char ***cmd, t_tok *token, int *i)
{
	int	size;
	int	pos;

	size = psr_cmd_count_words(token);
	*cmd = malloc((size + 1) * sizeof(char *));
	if (*cmd == NULL)
		return (-1);
	pos = 0;
	while (token->type == TOK_WORD)
	{
		(*cmd)[pos] = fthat_malloc_expands_remove_quotes(token->start, token->len);
		if ((*cmd)[pos++] == NULL)
		{
			// a function that destroys the array
			return (-1);
		}
		token++;
	}
	(*cmd)[size] = NULL;
	*i += (size - 1);
	return (0);
}

// 10/09
int	psr_parse_line(char *line, t_core *core)
{
	t_tok	tokens[TOK_MAX];
	t_tok	*token;
	int		i;
	t_cmd	*cmd;

	if (lex_tokenize_line(line, tokens) || psr_error_check(tokens) == -1)
		return (-1);

	// Somewhere else please?
	ft_bzero(core->cmds, CMD_MAX * sizeof(t_cmd));
	core->cmd_nbr = 0;

	i = 0;
	cmd = core->cmds + core->cmd_nbr;
	token = tokens + i;

	// Please jeffrey-ize me that shit. Oh my god.
	while (token->type != TOK_EOL)
	{
		if (token->type == TOK_WORD) 
		{
			if (psr_add_cmd(&cmd->cmd, token, &i) == -1)
				return (-1);
		}
		else if (token->type == TOK_IRED || token->type == TOK_IRED_HD) // Not quite good enough.
		{
			if (psr_add_red(cmd->ireds, &cmd->ilen, token, &i) == -1)
				return (-1);
		}
		else if (token->type == TOK_ORED || token->type == TOK_ORED_AP)
		{
			if (psr_add_red(cmd->oreds, &cmd->olen, token, &i) == -1)
				return (-1);
		}
		else if (token->type == TOK_PIPE)
		{
			cmd = core->cmds + ++core->cmd_nbr;
		}
		token = tokens + ++i;
	}
	return (0);
}

// static int	lex_print_tokens(t_tok *tokens)
// {
// 	int		i;
// 	t_tok	*token;

// 	i = -1;
// 	while ((token = tokens + ++i)->type != TOK_EOL)
// 	{
// 		if (token->type == TOK_WORD)
// 		{
// 			write(1, "\tTOK_WORD: ", 11);
// 			write(1, token->start, token->len);
// 			write(1, "\n", 1);
// 		}
// 		else if (token->type == TOK_IRED)
// 			printf("\tTOK_IRED\n");
// 		else if (token->type == TOK_IRED_HD)
// 			printf("\tTOK_IRED_HD\n");
// 		else if (token->type == TOK_ORED)
// 			printf("\tTOK_ORED\n");
// 		else if (token->type == TOK_ORED_AP)
// 			printf("\tTOK_ORED_AP\n");
// 		else if (token->type == TOK_PIPE)
// 			printf("\tTOK_PIPE\n");
// 	}
// 	return (0);
// }