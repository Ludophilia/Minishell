/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 17:21:01 by jgermany          #+#    #+#             */
/*   Updated: 2022/12/17 01:29:35 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int nb, int fd)
{
	if (fd < 0)
		return ;
	if (nb >= 10 || nb <= -10)
		ft_putnbr_fd(nb / 10, fd);
	if (nb < 0 && nb > -10)
		ft_putchar_fd('-', fd);
	if (nb < 0)
		nb = '0' + -(nb % 10);
	else
		nb = '0' + (nb % 10);
	ft_putchar_fd(nb, fd);
}
