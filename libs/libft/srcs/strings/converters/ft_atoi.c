/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:33:24 by jgermany          #+#    #+#             */
/*   Updated: 2022/12/13 16:26:50 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	return (((c >= '\x9') && (c <= '\xD')) || (c == '\x20'));
}

static int	is_sign(char c)
{
	return ((c == '+') || (c == '-'));
}

int	ft_atoi(const char *nptr)
{
	int	res;
	int	s;
	int	i;

	res = 0;
	s = 1;
	i = -1;
	while (nptr[++i])
	{
		if (!(is_space(nptr[i]) || is_sign(nptr[i]) || ft_isdigit(nptr[i])))
			break ;
		else if (is_sign(nptr[i]) && !ft_isdigit(nptr[i + 1]))
			break ;
		if (nptr[i] == '-')
			s *= -1;
		else if (ft_isdigit(nptr[i]))
			res = 10 * res + (nptr[i] - '0');
		if (ft_isdigit(nptr[i]) && !ft_isdigit(nptr[i + 1]))
			return (s * res);
	}
	return (0);
}
