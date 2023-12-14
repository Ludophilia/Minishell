/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:36:16 by jgermany          #+#    #+#             */
/*   Updated: 2023/01/01 20:36:55 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void		*arr;

	if ((size != 0) && (nmemb != ((nmemb * size) / size)))
		return ((void *)0);
	arr = malloc(nmemb * size);
	if (arr)
		ft_bzero(arr, nmemb * size);
	return (arr);
}
