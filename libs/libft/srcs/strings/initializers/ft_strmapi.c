/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 14:55:29 by jgermany          #+#    #+#             */
/*   Updated: 2022/12/29 14:30:58 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*ns;
	int		i;

	if (s == (char *)0)
		return ((char *)0);
	ns = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!ns)
		return ((char *)0);
	i = -1;
	while (s[++i])
		ns[i] = f(i, s[i]);
	ns[i] = '\x0';
	return (ns);
}
