/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:13:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/11 15:02:59 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*exp_alloc_word(char *start, int len, t_tokt context, t_core *core)
{
	int		size;
	int		quoted;
	int		j;

	size = 0;
	j = 0;
	quoted = 0;
	while (j < len)
	{
		if (exp_is_outq(start[j], &quoted) && ++j)
			continue ;
		if (exp_is_envv(start + j, context, quoted))
			size += exp_envv_value_len(start + j, &j, core);
		else
			(void)(j++, size++);
	}
	return (ft_calloc(size + 1, sizeof(char)));
}

static char	*exp_create_word(t_tok *tok, t_tokt context, t_core *core)
{
	char	*word;
	int		quoted;
	int		i;
	int		j;

	word = exp_alloc_word(tok->start, tok->len, context, core);
	if (word == NULL)
		return (NULL);
	i = 0;
	j = 0;
	quoted = 0;
	while (i < tok->len)
	{
		if (exp_is_outq(tok->start[i], &quoted) && ++i)
			continue ;
		if (exp_is_envv(tok->start + i, context, quoted))
			i += exp_copy_envv_value(tok->start + i, word, &j, core);
		else
			word[j++] = tok->start[i++];
	}
	return (word);
}

int	exp_cnsm_rtoks(t_cmd *cmd, t_core *core)
{
	t_red	*red;
	int		i;

	i = 0;
	red = cmd->reds + i;
	while (red->type)
	{
		red->word = exp_create_word(red->word_tk, red->type, core);
		if (red->word == NULL)
			return (-1);
		red = cmd->reds + ++i;
	}
	return (0);
}

int	exp_cnsm_wtoks(t_cmd	*cmd, t_core *core)
{
	t_tok	*tok;
	int		i;

	cmd->argv = ft_calloc(cmd->argc + 1, sizeof(char *));
	if (cmd->argv == NULL)
		return (-1);
	i = 0;
	tok = cmd->argv_tk[i];
	while (tok)
	{
		// 5/01: Why this??? Why is "" filtered?
		// if (*word != 0)
		// cmd->argv[cmd->argc++] = word;
		cmd->argv[i] = exp_create_word(tok, TOK_WORD, core);
		if (cmd->argv[i] == NULL)
			return (-1);
		// printf("argv[%i] -> %s\n", i, cmd->argv[i]);
		// else
		// 	free(word);
		tok = cmd->argv_tk[++i];
	}
	return (0);
}
