/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferer_helpers_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:24:50 by jegerman          #+#    #+#             */
/*   Updated: 2024/12/21 16:10:15 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static int	buffer_str(char *str, char *buffer, int *i)
{
	while (*str)
		buffer[(*i)++] = *str++;
	return (1);
}

static int	buffer_char(char c, char *buffer, int *i)
{
	buffer[(*i)++] = c;
	return (1);
}

int	buffer_nbr_base(t_nbr *nbr, char *buffer, int *i, t_meta *m)
{
	size_t	radix;
	t_nbr	*tmp;

	if (flag(CV_PTR, m) && nbr->magn == 0 && buffer_str("(nil)", buffer, i))
		return (1);
	radix = ft_strlen(nbr->base);
	tmp = &(t_nbr){.magn = nbr->magn / radix, .sign = nbr->sign,
		.base = nbr->base};
	if (nbr->magn >= radix)
		buffer_nbr_base(tmp, buffer, i, m);
	buffer_char(nbr->base[nbr->magn % radix], buffer, i);
	return (1);
}
