/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:26:51 by jegerman          #+#    #+#             */
/*   Updated: 2024/12/21 16:10:15 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

bool	is_valid_flg(char c)
{
	if (c == '-' || c == '0' || c == '#' || c == ' ' || c == '+')
		return (1);
	return (0);
}

bool	is_valid_conv(char c)
{
	if (c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i'
		|| c == 'u' || c == 'x' || c == 'X' || c == '%')
		return (1);
	return (0);
}

int	set_conv(char c, t_meta *m)
{
	if (c == 'c')
		m->flags |= CV_CHR;
	else if (c == 's')
		m->flags |= CV_STR;
	else if (c == 'p')
		m->flags |= CV_PTR;
	else if (c == 'd' || c == 'i')
		m->flags |= CV_INT;
	else if (c == 'u')
		m->flags |= CV_UINT;
	else if (c == 'x' || c == 'X')
		m->flags |= CV_HEX;
	else if (c == '%')
		m->flags |= CV_PRC;
	if (c == 'x')
		m->flags |= CV_HEXL;
	else if (c == 'X')
		m->flags |= CV_HEXU;
	return (1);
}
