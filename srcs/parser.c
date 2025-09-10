/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:16:27 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/10 19:03:09 by jegerman         ###   ########.fr       */
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

// 11/09 - not_impl_function uses malloc and stuff...

int	psr_add_red(t_red *reds, int *len, t_tok *token, int *i)
{
	t_red	*red;
	
	red = reds + *len;
	red->type = token->type;
	red->word = not_impl_function(token[1].start, token[1].len);
	if (red->word == NULL)
		return (-1);
	*i += 1;
	*len += 1;
	return (0);
}

int	psr_parse_line(char *line, t_core *core)
{
	t_tok	tokens[TOK_MAX];
	t_tok	*token;
	int		i;
	t_cmd	*cmd;

	if (lex_tokenize_line(line, tokens) || psr_error_check(tokens) == -1)
		return (-1);
	ft_bzero(core->cmds, CMD_MAX * sizeof(t_cmd));
	core->cmd_nbr = 0;

	i = 0;
	cmd = core->cmds + core->cmd_nbr;
	token = tokens + i;
	while (token->type != TOK_EOL)
	{
		if (token->type == TOK_WORD && )
			// ;
		if (token->type == TOK_PIPE)
			cmd = core->cmds + ++core->cmd_nbr;
		if (((token->type == TOK_IRED || token->type == TOK_IRED_HD) // hard to read...
				&& psr_add_red(cmd->ireds, &cmd->ilen, token, &i))
			|| ((token->type == TOK_ORED || token->type == TOK_ORED_AP)
				&& psr_add_red(cmd->oreds, &cmd->olen, token, &i)))
			return (-1);

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