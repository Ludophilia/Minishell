/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 19:27:18 by jgermany          #+#    #+#             */
/*   Updated: 2022/12/21 17:00:30 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_lennbr(int n)
{
	size_t	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		sgn;
	size_t	len;
	size_t	i;

	len = ft_lennbr(n);
	sgn = 1;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return ((char *)0);
	str[len] = '\x0';
	if (n < 0)
		str[0] = '-';
	i = 0;
	while (n || !i)
	{
		if (n < 0)
			sgn = -1;
		str[len - 1 - i++] = '0' + sgn * (n % 10);
		n /= 10;
	}
	return (str);
}
