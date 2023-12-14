/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 14:59:24 by jgermany          #+#    #+#             */
/*   Updated: 2022/12/29 14:45:02 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_charstr(char c, char const *set)
{
	int	i;

	i = -1;
	while (set[++i])
		if (c == set[i])
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	start;
	size_t	end;

	if (s1 == (char *)0 || set == (char *)0)
		return ((char *)0);
	start = 0;
	end = ft_strlen(s1) - 1;
	while (s1[start])
	{
		if (!ft_charstr(s1[start], set))
			break ;
		start++;
	}
	while (s1[end] && (end != start))
	{
		if (!ft_charstr(s1[end], set))
			break ;
		end--;
	}
	str = ft_substr(s1, start, (end + 1) - start);
	return (str);
}
