/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 18:56:25 by jgermany          #+#    #+#             */
/*   Updated: 2022/12/18 16:30:02 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	int		i;

	str = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!str)
		return ((char *)0);
	i = -1;
	while (s[++i])
		str[i] = s[i];
	str[i] = '\x0';
	return (str);
}
