/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/09 19:13:59 by jegerman         ###   ########.fr       */
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

int	psr_parse_line(char *line, t_core *core)
{
	t_tok	tokens[TOK_MAX];

	(void)core;
	if (lex_tokenize_line(line, tokens) || psr_error_check(tokens) == -1)
		return (-1);

	// 7/09 - What's next?
	// 	- We have a clean list of tokens

	//	- Now we have to...
	// 		- [Copy tokens in project structures.] (goal: -> execute commands)

	//			- # Problem #1 - We don't know what does the project structure 
	//			look like we don't know how to structure that struct
	/*
					- ## What should be stored (from that list of tokens) and why?
					 (good question.)
						- the WORD that contains the command
							- the WORD(s) which indicate the IRED or IRED_HD
								- IRED is not opened in the parent
								- but IRED_HD is, stored in a pipe.
							- the WORD(s) which indicate the ORED or ORED_AP
								- ORED or ORED_AP are not opened in the parent

						- the WORD that contains the next command
							- ...

					- ## How to store all of this...
						- Array of structures that represents commands (seems correct)
							- char *command for the WORD, expanded if necessary, cleaned from quotes
								- TOK_PIPE -> next command
								- TOK_EOL -> no more command
			
							- ... [what's next is the real deal]

							- for redirections, it's more complicated.
								- most redirections (<, >, >>) should be just stored as strings
								files will be opened in child
								- here docs will be opened parent/parser level and... stored
								in a pipe.

	*/
	
	//			- Problem #2 - 
	// 				- [We also have that expansion and quote cleanup problem...]


	

	// - iterate on tokens
		// - 
	//	- 
	
	
	lex_print_tokens(tokens);
	return (0);
}


