/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:13:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/09 19:48:13 by jegerman         ###   ########.fr       */
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

char	*exp_create_word(t_tok *tok, t_tokt context, t_core *core)
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
