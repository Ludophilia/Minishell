/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/05 18:30:45 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 5/09 - 
int	psr_parse_line(char *line, t_core *core)
{
	t_tok	tokens[TOK_MAX]; // In the general, projet-level, struct? I don't think it's necessary...

	(void)core;
	lex_tokenize_line(line, tokens);
	
	return (0);
}
