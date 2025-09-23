/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:18:43 by jegerman          #+#    #+#             */
/*   Updated: 2025/08/31 18:44:40 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	hdn_is_in_set(const char c1, const char *set)
{
	while (*set)
		if (c1 == *set++)
			return (1);
	return (0);
}

static ssize_t	hdn_get_trim_size(char const *s1, char const *set, ssize_t *i)
{
	ssize_t	k;

	k = ft_strlen(s1) - 1;
	if (*set == 0)
		return (k + 1);
	while (s1[*i] && hdn_is_in_set(s1[*i], set))
		++*i;
	while (k >= 0 && k >= *i && hdn_is_in_set(s1[k], set))
		--k;
	if (k < *i)
		return (0);
	return (k - *i + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	ssize_t	size;
	ssize_t	i;
	ssize_t	j;
	char	*trim;

	if (s1 == NULL || set == NULL)
		return (NULL);
	i = 0;
	size = hdn_get_trim_size(s1, set, &i);
	trim = ft_calloc(size + 1, sizeof(char));
	if (trim == NULL)
		return (NULL);
	j = -1;
	while (++j < size)
		trim[j] = s1[i + j];
	return (trim);
}
