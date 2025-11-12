/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:11:36 by jegerman          #+#    #+#             */
/*   Updated: 2024/12/21 16:26:53 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static void	process_character_specifier(t_meta *m)
{
	int	c;

	c = '%';
	if (flag(CV_CHR, m))
		c = va_arg(m->args, int);
	if (flag(FLG_FIEL, m))
		process_filler_width(NULL, m);
	if (flags(FLG_FIEL, m) && not_flag(FLG_DASH, m))
		print_filler(SPACEFILL, m);
	ft_putchar_cc(c, m);
	if (flags(FLG_FIEL | FLG_DASH, m))
		print_filler(SPACEFILL, m);
}

static void	process_string_specifier(t_meta *m)
{
	char	*str;
	int		i;

	str = va_arg(m->args, char *);
	if (str == NULL && flag(FLG_PREC, m) && m->prec_v < 6)
		str = "";
	else if (str == NULL)
		str = "(null)";
	if (flag(FLG_FIEL, m))
		process_filler_width(str, m);
	if (flag(FLG_FIEL, m) && not_flag(FLG_DASH, m))
		print_filler(SPACEFILL, m);
	i = 0;
	while (flag(FLG_PREC, m) && str[i] && i < m->prec_v)
		ft_putchar_cc(str[i++], m);
	if (not_flag(FLG_PREC, m))
		ft_putstr_cc(str, m);
	if (flags(FLG_FIEL | FLG_DASH, m))
		print_filler(SPACEFILL, m);
}

static void	process_number_specifier(t_meta *m)
{
	t_nbr	nbr;

	if (set_magnitude_buffer(&nbr, m) == NULL)
		return ;
	if (flag(FLG_FIEL, m))
		process_filler_width(&nbr, m);
	if (flags(FLG_ZERO, m))
		print_prefix(&nbr, m);
	if (flags(FLG_FIEL | FLG_ZERO, m))
		print_filler(ZEROFILL, m);
	if (flag(FLG_FIEL, m) && not_flag(FLG_ZERO | FLG_DASH, m))
		print_filler(SPACEFILL, m);
	if (not_flag(FLG_ZERO, m))
		print_prefix(&nbr, m);
	ft_putstr_cc(nbr.pad_magn, m);
	if (flag(FLG_DASH, m))
		print_filler(SPACEFILL, m);
	free(nbr.pad_magn);
}

void	process_specifier(t_meta *m)
{
	if (flag(CV_CHR | CV_PRC, m))
		process_character_specifier(m);
	else if (flag(CV_STR, m))
		process_string_specifier(m);
	else if (flag(CV_HEX | CV_INT | CV_PTR | CV_UINT, m))
		process_number_specifier(m);
	*m->i += 1;
}
