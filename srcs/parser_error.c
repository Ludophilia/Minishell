/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 13:37:44 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/13 19:07:58 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_optk_check(t_tok *token, int pos)
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

static int	ps_wtk_check(t_tok *token)
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
