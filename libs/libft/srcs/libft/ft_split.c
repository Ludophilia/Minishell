/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:30:31 by jegerman          #+#    #+#             */
/*   Updated: 2025/08/31 18:44:40 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	hdn_get_nb_of_words(const char *s, char c)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			++i;
		if (s[i] && s[i] != c)
			++words;
		while (s[i] && s[i] != c)
			++i;
	}
	return (words);
}

static char	*hdn_create_word(const char *s, char c, int *i, char **strs)
{
	char	*word;
	int		size;
	int		j;

	size = 0;
	while (s[size] && s[size] != c)
		++size;
	word = ft_calloc(size + 1, sizeof(char));
	if (word == NULL)
		return (NULL);
	j = -1;
	while (++j < size)
		word[j] = s[j];
	*i += size;
	*strs = word;
	return (word);
}

static int	hdn_free_strs(char **strs, int j)
{
	while (--j >= 0)
		free(strs[j]);
	free(strs);
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**strs;
	int		i;
	int		j;

	if (s == NULL)
		return (NULL);
	words = hdn_get_nb_of_words(s, c);
	strs = ft_calloc(words + 1, sizeof(char *));
	if (strs == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			++i;
		if (s[i] && s[i] != c
			&& hdn_create_word(s + i, c, &i, strs + j++) == NULL
			&& hdn_free_strs(strs, j - 1))
			return (NULL);
	}
	return (strs);
}
