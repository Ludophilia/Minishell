/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:24:27 by jgermany          #+#    #+#             */
/*   Updated: 2023/05/08 17:00:18 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printer.h"

void	print_char(char c, t_meta *meta)
{
	putchar_cc(c, meta);
}

void	print_str(char *str, t_meta *meta)
{
	if (!str)
		str = "(null)";
	putstr_cc(str, meta);
}

void	print_nbr(t_nbr nb, char *base, t_flag *flags, t_meta *meta)
{
	unsigned long	radix;

	if (nb.abs == 0 && flags->conv_t == 'p')
		return (putstr_cc("(nil)", meta));
	radix = ft_strlen(base);
	if (nb.abs >= radix)
		print_nbr((t_nbr){nb.sign, nb.abs / radix}, base, flags, meta);
	if (nb.abs < radix && nb.sign)
		putchar_cc('-', meta);
	else if (nb.abs < radix && flags->conv_t == 'p')
		putstr_cc("0x", meta);
	putchar_cc(base[nb.abs % radix], meta);
}
