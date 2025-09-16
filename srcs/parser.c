/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/16 22:53:52 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #########################################################

// WORD CREATION, QUOTES REMOVAL, VARIABLE EXPANSION

// #########################################################
// parser_expander


char	*psr_get_envv_value(char *start, int *envv_len)
{
	char	envv[ID_LMAX];
	char	*envv_val;
	int		len;

	++start;
	len = -1;
	while (++len, lex_is_envv_chr(start[len], len))
		envv[len] = start[len];
	envv[len] = '\0';
	envv_len && (*envv_len = len + 1);
	envv_val = getenv(envv); // or custom funct
	return (envv_val);
}

int	psr_get_envv_val_size(char *start, int *j)
{
	char	*envv_val;
	int		size;
	int		envv_len;

	envv_val = psr_get_envv_value(start, &envv_len);
	if (envv_val == NULL)
		size = 0;
	else
		size = ft_strlen(envv_val);
	*j += envv_len;
	return (size);
}

int	psr_copy_envv_value(char *start, int *i, char *word, int *j)
{
	// - routine to get and copy the expanded variable
	// or nothing if doesn't exist
	// move j or don't move it all 
	// move i past the envv section
}

//###########################################################
// parser_utils

int	psr_is_outq(int c, int *quoted)
{
	if ((*quoted == 0 && lex_is_quote(c)) || (*quoted != 0 && *quoted == c))
		return ((*quoted == 0 && (*quoted = c)) || !(*quoted = 0));
	return (0);
}

int	psr_is_envv(char *c, int ct, int q)
{
	if (*c == '$' && ct != TOK_IRED_HD && q != '\'' && lex_is_envv_chr(c[1], 0))
		return (1);
	return (0);
}

// ##################################################################
// parser_core ?

char	*psr_alloc_word(char *start, int len, t_tokt context)
{
	int		size;
	int		quoted;
	int		j;

	size = 0;
	j = 0;
	quoted = 0;
	while (j < len)
	{
		if (psr_is_outq(start[j], &quoted) && ++j)
			continue ;
		if (psr_is_envv(start[j], context, quoted))
			size += psr_get_envv_val_size(start + j, &j);
		else
			j++, size++;
	}
	return (ft_calloc((size + 1), sizeof(char)));
}

// That WORD is the thing that will be
// - Part of argv if the word is part of the command
// - Part of a redirection 
// - Part of a delimiter for a HERE document

char	*psr_create_word(t_tok *token, t_tokt context)
{
	char	*word;
	int		i;
	int		j;
	int		quoted;

	word = psr_alloc_word(token->start, token->len, context);
	if (word == NULL)
		return (NULL);

	j = 0; // it's for the new word.
	i = 0;
	while (i < token->len) // this if is for the new word.
	{
		if (psr_is_outq(token->start[i], &quoted) && ++i)
			continue ;
		if (psr_is_envv(token->start + i, context, quoted))
			psr_copy_envvv(token->start, &i, word, &j);
		else
			word[j++] = token->start[i++];
	}
	return (word);
}

// NO EXPANSION FOR HERE DOCUMENTS!
	
// 	- Copy token byte by byte following the rules...
// 		- skip outer quotes if exist
//		- keep inner quotes if exist
//		- use expanded variables

// #########################################################
// parser_core

// MAIN PARSING ROUTINE

int	psr_parse_line(char *line, t_core *core)
{
	t_tok	tokens[TOK_MAX];
	t_tok	*token;
	t_cmd	*cmd;

	if (lex_tokenize_line(line, tokens) || psr_error_check(tokens) == -1)
		return (-1);
	ft_bzero(core->cmds, CMD_MAX * sizeof(t_cmd));
	core->cmd_nbr = 0;
	cmd = core->cmds + core->cmd_nbr;
	token = tokens;
	while (token->type != TOK_EOL)
	{
		// Where are the routines for destroying data?
		if (psr_add_cmd(token, cmd) == -1 || psr_add_reds(token, cmd) == -1)
			return (-1);
		while (token->type != TOK_PIPE && token->type != TOK_EOL)
			token++;
		if (token->type == TOK_PIPE)
			cmd = (++token, core->cmds + ++core->cmd_nbr);
		// Nothing else to add once the structures are full?
	}
	return (0);
}
