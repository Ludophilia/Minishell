/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf_commons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 14:23:44 by jgermany          #+#    #+#             */
/*   Updated: 2023/06/27 18:17:13 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf_commons.h"

void	putchar_cc(char c, t_meta *meta)
{
	ft_putchar_fd(c, meta->fd);
	meta->count += 1;
}

void	putstr_cc(char *s, t_meta *meta)
{
	ft_putstr_fd(s, meta->fd);
	meta->count += ft_strlen(s);
}
