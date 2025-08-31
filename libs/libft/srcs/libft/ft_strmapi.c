/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:41:34 by jegerman          #+#    #+#             */
/*   Updated: 2025/08/31 18:44:40 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*mpd_str;
	unsigned int	i;

	if (s == NULL || f == NULL)
		return (NULL);
	mpd_str = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (mpd_str == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		mpd_str[i] = f(i, s[i]);
		++i;
	}
	return (mpd_str);
}
