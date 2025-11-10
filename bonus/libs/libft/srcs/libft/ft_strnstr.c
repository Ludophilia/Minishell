/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:55:39 by jegerman          #+#    #+#             */
/*   Updated: 2024/11/13 15:45:58 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

static int	is_little(const char *big, const char *little, size_t bytes)
{
	while (bytes-- && *big && *big == *little)
	{
		if (*(little + 1) == '\0')
			return (1);
		++big;
		++little;
	}
	return (0);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	if (*little == '\0')
		return ((char *)big);
	i = 0;
	while (big[i] && i < len)
	{
		if (is_little(big + i, little, len - i))
			return ((char *)big + i);
		++i;
	}
	return (NULL);
}
