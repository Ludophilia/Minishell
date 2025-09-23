/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:13:16 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/23 15:46:19 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*psr_get_envv_value(char *start, int *envv_len)
{
	static char	exitw[EXW_MAX];
	char		envv[ID_LMAX];
	char		*envv_val;
	int			len;

	++start;
	len = -1;
	if (*start == '?' && len++)
		envv[len++] = *start;
	else
		while (++len, psr_is_envv_chr(start[len], len))
			envv[len] = start[len];
	envv[len] = '\0';
	(void)(envv_len && (*envv_len = len + 1)); // remove void
	if (ft_strncmp(envv, "?", 2) == 0)
		envv_val = (utl_shitoa(g_exit_status, exitw), exitw); // correct?
	else
		envv_val = getenv(envv); // or custom funct
	return (envv_val);
}

static int	psr_get_envv_val_size(char *start, int *j)
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

static int	psr_copy_envv_value(char *start, int *i, char *word, int *j)
{
	char	*envv_val;
	int		envv_len;

	envv_val = psr_get_envv_value(start, &envv_len);
	if (envv_val == NULL)
		return (*i += envv_len, 0);
	while (*envv_val)
		word[(*j)++] = *envv_val++;
	return (*i += envv_len, 0);
}

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
			size += psr_get_envv_val_size(start + j, &j);
		else
			(j++, size++);
	}
	return (ft_calloc(size + 1, sizeof(char)));
}

char	*psr_create_word(t_tok *tok, t_tokt context)
{
	char	*word;
	int		i;
	int		j;
	int		quoted;

	word = psr_alloc_word(tok->start, tok->len, context);
	if (word == NULL)
		return (NULL);
	j = 0;
	i = 0;
	quoted = 0;
	while (i < tok->len)
	{
		if (psr_is_outq(tok->start[i], &quoted) && ++i)
			continue ;
		if (psr_is_envv(tok->start + i, context, quoted))
			psr_copy_envv_value(tok->start + i, &i, word, &j);
		else
			word[j++] = tok->start[i++];
	}
	return (word);
}
