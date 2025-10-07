/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:13:16 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/07 18:49:08 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*psr_alloc_word(char *start, int len, t_tokt context)
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
		if (psr_is_envv(start + j, context, quoted))
			size += psr_envv_value_len(start + j, &j);
		else
			(void)(j++, size++);
	}
	return (ft_calloc(size + 1, sizeof(char)));
}

char	*psr_create_word(t_tok *tok, t_tokt context)
{
	char	*word;
	int		quoted;
	int		i;
	int		j;

	word = psr_alloc_word(tok->start, tok->len, context);
	if (word == NULL)
		return (NULL);
	i = 0;
	j = 0;
	quoted = 0;
	while (i < tok->len)
	{
		if (psr_is_outq(tok->start[i], &quoted) && ++i)
			continue ;
		if (psr_is_envv(tok->start + i, context, quoted))
			i += psr_copy_envv_value(tok->start + i, word, &j);
		else
			word[j++] = tok->start[i++];
	}
	return (word);
}
