/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:11:12 by jegerman          #+#    #+#             */
/*   Updated: 2025/02/11 17:09:58 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_putchar_cc(const char c, t_meta *m)
{
	write(m->fd, &c, 1);
	m->count++;
	return (1);
}

int	ft_putstr_cc(const char *str, t_meta *m)
{
	int	len;

	len = ft_strlen(str);
	if (len == 0)
		return (1);
	write(m->fd, str, len);
	m->count += len;
	return (1);
}

void	print_filler(bool zfill, t_meta *m)
{
	while (m->field_v-- > 0)
		ft_putchar_cc((char [2]){' ', '0'}[zfill], m);
}

int	print_prefix(t_nbr *nbr, t_meta *m)
{
	if (nbr->sign == 1 && flag(CV_INT, m))
		ft_putchar_cc('-', m);
	else if (flags(CV_INT | FLG_PLUS, m))
		ft_putchar_cc('+', m);
	else if (flags(CV_INT | FLG_SPAC, m))
		ft_putchar_cc(' ', m);
	else if (nbr->magn > 0 && (flags(CV_HEXL | FLG_POUN, m) || flag(CV_PTR, m)))
		ft_putstr_cc("0x", m);
	else if (nbr->magn > 0 && flags(CV_HEXU | FLG_POUN, m))
		ft_putstr_cc("0X", m);
	return (1);
}
