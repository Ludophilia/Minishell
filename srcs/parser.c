/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/13 19:09:41 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #########################################################

// WORD CREATION, QUOTES REMOVAL, VARIABLE EXPANSION

int	lex_is_cidentifier(char c, int pos)
{
	if (pos == 0)
		return (ft_isalpha(c) || c == '_');
	else
		return (ft_isalnum(c) || c == '_');
}

// 12/09 - Alright, what should be done?
int	psr_expand_envv(char *start, int *j, int len, char **expanded)
{
	char	e_var[ID_LMAX];

	// copy the token start + len to e_var
	//	- Sure, but why?

	// expand the identifier to its real value.


	// NO EXPANSION FOR HERE DOCUMENTS!

	return ();
}

// Count the number of characters needed to create the word from its token
// - Includes outer quote removal
// - Include variable expansions
int	psr_count_chrs(char *start, int len, char **expanded)
{
	int		size;
	char	quoted;
	int		j;

	size = 0;
	j = 0;
	quoted = 0;
	while (j < len)
	{
		// Move that into a funct.
		if ((!quoted && lex_is_quote(start[j])) || (quoted && start[j] == quoted))
		{
			((!quoted && (quoted = start[j])) || (quoted = 0));
			j++;
			continue ;
		}
		if (start[j] == '$' && quoted != '\'' && lex_is_cidentifier(start[j + 1], 0))
			size += psr_expand_envv(start, &j, len, expanded);
		else
			j++, size++;
	}
	return (size);
}

// Creates the word from TOK_WORD:
//	== without outer quotes (if needed)
// 	== with variable expansion (if needed)
char	*psr_create_word(t_tok *token)
{
	char	*word;
	int		size;
	int		i;
	char	*expanded;

	size = psr_count_chrs(token->start, token->len, &expanded);
	word = malloc((size + 1) * sizeof(char));
	if (word == NULL)
		return (NULL);
	i = -1;
	// NO EXPANSION FOR HERE DOCUMENTS!
	while (++i < size)
	{
	// 	- Copy token byte by byte following the rules...
	// 		- skip outer quotes if exist
	//		- keep inner quotes if exist
	//		- use expanded variables 
	}
	word[size] = 0;
	return (word);
}


// #########################################################

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
