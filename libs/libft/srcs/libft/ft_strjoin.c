/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:48:43 by jegerman          #+#    #+#             */
/*   Updated: 2025/08/31 18:44:40 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len[2];
	char	*s1s2;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len[0] = ft_strlen(s1);
	len[1] = ft_strlen(s2);
	s1s2 = ft_calloc(len[0] + len[1] + 1, sizeof(char));
	if (s1s2 == NULL)
		return (NULL);
	if (ft_strlcat(s1s2, s1, len[0] + 1) == len[0]
		&& ft_strlcat(s1s2, s2, len[0] + len[1] + 1) == len[0] + len[1])
		return (s1s2);
	return (NULL);
}
