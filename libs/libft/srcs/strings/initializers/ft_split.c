/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 13:08:05 by jgermany          #+#    #+#             */
/*   Updated: 2023/08/12 13:47:59 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wordslen(char **strs)
{
	size_t	len;

	len = 0;
	while (strs[len])
		len++;
	return (len);
}

static char	**ft_create_word(char *s)
{
	char	**strs;
	size_t	len;
	int		i;

	len = 0;
	if (*s)
		len++;
	strs = (char **)malloc((len + 1) * sizeof(char *));
	if (!strs)
		return ((char **)0);
	i = 0;
	while (len--)
		strs[i++] = ft_strdup(s);
	strs[i] = (char *)0;
	return (strs);
}

static char	**ft_join_words(char **strs1, char **strs2)
{
	char	**strs;
	size_t	lens1;
	size_t	i;

	lens1 = ft_wordslen(strs1);
	strs = (char **)malloc((lens1 + ft_wordslen(strs2) + 1) * sizeof(char *));
	if (!strs)
		return ((char **)0);
	i = 0;
	while (strs1[i])
	{
		strs[i] = strs1[i];
		i++;
	}
	while (strs2[i - lens1])
	{
		strs[i] = strs2[i - lens1];
		i++;
	}
	strs[i] = (char *)0;
	free(strs1);
	free(strs2);
	return (strs);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	char	*dup;
	char	*sep;

	if (s == (char *)0)
		return ((char **)0);
	dup = ft_strdup(s);
	sep = ft_memchr(dup, c, ft_strlen(dup));
	if (sep)
	{
		*sep = '\x0';
		strs = ft_join_words(ft_split(dup, c), ft_split(sep + 1, c));
	}
	else
		strs = ft_create_word(dup);
	free(dup);
	return (strs);
}
