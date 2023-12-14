/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 21:12:12 by jgermany          #+#    #+#             */
/*   Updated: 2023/06/27 18:36:38 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	is_valid_type(char c)
{
	if (c == 'c' || c == '%' || c == 's' || c == 'd' || c == 'i'
		|| c == 'u' || c == 'x' || c == 'X' || c == 'p')
		return (1);
	return (0);
}

static void	proc_char_conv(char *spec, va_list *args, t_meta *meta)
{
	if (*spec == 'c')
		print_char(va_arg(*args, int), meta);
	else if (*spec == '%')
		print_char('%', meta);
	else if (*spec == 's')
		print_str(va_arg(*args, char *), meta);
}

static void	proc_nbr_conv(char *spec, va_list *args, t_flag *flags,
t_meta *meta)
{
	long			snbr;
	unsigned long	unbr;

	snbr = 0;
	unbr = 0;
	if (*spec == 'd' || *spec == 'i')
		snbr = va_arg(*args, int);
	else if (*spec == 'u' || *spec == 'x' || *spec == 'X')
		unbr = va_arg(*args, unsigned int);
	else if (*spec == 'p')
		unbr = va_arg(*args, uintptr_t);
	if (snbr < 0 && (*spec == 'd' || *spec == 'i'))
		print_nbr((t_nbr){1, -snbr}, B10, flags, meta);
	else if (snbr >= 0 && (*spec == 'd' || *spec == 'i'))
		print_nbr((t_nbr){0, snbr}, B10, flags, meta);
	else if (*spec == 'u')
		print_nbr((t_nbr){0, unbr}, B10, flags, meta);
	else if (*spec == 'x')
		print_nbr((t_nbr){0, unbr}, B16L, flags, meta);
	else if (*spec == 'X')
		print_nbr((t_nbr){0, unbr}, B16U, flags, meta);
	else if (*spec == 'p')
		print_nbr((t_nbr){0, unbr}, B16L, flags, meta);
}

int	proc_specif(char *spec, va_list *args, t_meta *meta)
{
	t_flag	*flags;

	if (!is_valid_type(*spec))
	{
		errno = EINVAL;
		return (-1);
	}
	flags = ft_calloc(1, sizeof(t_flag));
	if (!flags)
		return (-1);
	flags->conv_t = *spec;
	if (*spec == 'c' || *spec == '%' || *spec == 's')
		proc_char_conv(spec, args, meta);
	else
		proc_nbr_conv(spec, args, flags, meta);
	free(flags);
	return (1);
}
