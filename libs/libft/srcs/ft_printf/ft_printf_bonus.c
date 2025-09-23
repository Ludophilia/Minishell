/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:58:13 by jegerman          #+#    #+#             */
/*   Updated: 2025/04/23 20:57:31 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_dprintf(int fd, const char *format, ...)
{
	t_meta	meta;
	int		i;

	if (format == NULL)
		return (-1);
	i = 0;
	meta = (t_meta){.i = &i, .fd = fd, .count = 0};
	va_start(meta.args, format);
	while (format[i])
	{
		if (format[i] == '%' && is_valid_specif(format + ++i, &meta))
			process_specifier(&meta);
		else
			ft_putchar_cc(format[i++], &meta);
	}
	va_end(meta.args);
	return (meta.count);
}

int	ft_eprintf(const char *format, ...)
{
	t_meta	meta;
	int		i;

	if (format == NULL)
		return (-1);
	i = 0;
	meta = (t_meta){.i = &i, .fd = 2, .count = 0};
	va_start(meta.args, format);
	while (format[i])
	{
		if (format[i] == '%' && is_valid_specif(format + ++i, &meta))
			process_specifier(&meta);
		else
			ft_putchar_cc(format[i++], &meta);
	}
	va_end(meta.args);
	return (meta.count);
}

int	ft_printf(const char *format, ...)
{
	t_meta	meta;
	int		i;

	if (format == NULL)
		return (-1);
	i = 0;
	meta = (t_meta){.i = &i, .fd = 1, .count = 0};
	va_start(meta.args, format);
	while (format[i])
	{
		if (format[i] == '%' && is_valid_specif(format + ++i, &meta))
			process_specifier(&meta);
		else
			ft_putchar_cc(format[i++], &meta);
	}
	va_end(meta.args);
	return (meta.count);
}
