/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:02:22 by jegerman          #+#    #+#             */
/*   Updated: 2025/08/31 18:44:40 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static void	*ft_rev_memcpy(void *dest, const void *src, size_t n)
{
	if (n == 0)
		return (dest);
	while (--n + dest >= dest)
		((char *)dest)[n] = ((char *)src)[n];
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	t_mover	*movers;
	void	*ret_dest;

	movers = (t_mover[2]){ft_memcpy, ft_rev_memcpy};
	ret_dest = (movers[dest >= src])(dest, src, n);
	return (ret_dest);
}
