/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:28:28 by jgermany          #+#    #+#             */
/*   Updated: 2023/06/27 19:22:24 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

static int	process_format(const char *format, va_list *args, t_meta *meta)
{
	int		i;
	int		offset;

	i = -1;
	while (format[++i])
	{
		if (format[i] == '%')
		{
			offset = proc_specif((char *)format + i + 1, args, meta);
			if (offset == -1)
				return (-1);
			else
				i += offset;
		}
		else
			print_char(format[i], meta);
	}
	return (0);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	t_meta		meta;
	va_list		args;

	if (!format)
	{
		errno = EINVAL;
		return (-1);
	}
	meta.fd = fd;
	meta.count = 0;
	va_start(args, format);
	if (process_format(format, &args, &meta) == -1)
		return (-1);
	va_end(args);
	return (meta.count);
}

int	ft_printf(const char *format, ...)
{
	t_meta		meta;
	va_list		args;

	if (!format)
	{
		errno = EINVAL;
		return (-1);
	}
	meta.fd = 1;
	meta.count = 0;
	va_start(args, format);
	if (process_format(format, &args, &meta) == -1)
		return (-1);
	va_end(args);
	return (meta.count);
}
