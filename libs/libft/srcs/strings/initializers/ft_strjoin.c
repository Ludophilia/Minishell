/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 14:06:25 by jgermany          #+#    #+#             */
/*   Updated: 2022/12/29 14:29:07 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*dest;
	size_t		ls1;
	size_t		ls2;
	size_t		i;

	if (s1 == (char *)0 || s2 == (char *)0)
		return ((char *)0);
	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s2);
	dest = ft_calloc(ls1 + ls2 + 1, sizeof(char));
	if (dest)
	{
		i = ft_strlcat(dest, s1, (ls1 + 1));
		if (i == (0 + ls1))
			i = ft_strlcat(dest, s2, (ls1 + ls2 + 1));
		if (i == (ls1 + ls2))
			return (dest);
	}
	return ((char *)0);
}
