/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:45:17 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/28 17:55:12 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	main(void)
{
	if (write(1, "Write this on stdout!\n", 23) == -1
		|| write(2, "Write this on stderr!\n", 23) == -1)
		return (1);
	return (0);
}
