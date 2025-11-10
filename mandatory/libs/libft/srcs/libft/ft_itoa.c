/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:21:23 by jegerman          #+#    #+#             */
/*   Updated: 2025/08/31 18:44:40 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	hdn_get_nb_length(int n)
{
	size_t	len;

	len = 0;
	if (n <= 0)
		++len;
	while (n && ++len)
		n /= 10;
	return (len);
}

static char	*hdn_conv_nb_to_str(int n, char *str)
{
	if (n >= 10 || n <= -10)
		str = hdn_conv_nb_to_str(n / 10, str);
	if (n > -10 && n < 0)
		*str++ = '-';
	if (n < 0)
		*str++ = '0' + -(n % 10);
	else
		*str++ = '0' + (n % 10);
	return (str);
}

char	*ft_itoa(int n)
{
	char	*n_str;
	size_t	n_len;

	n_len = hdn_get_nb_length(n);
	n_str = ft_calloc(n_len + 1, sizeof(char));
	if (n_str == NULL)
		return (NULL);
	hdn_conv_nb_to_str(n, n_str);
	return (n_str);
}
