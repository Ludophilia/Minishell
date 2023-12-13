/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 21:26:41 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/02 15:48:58 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

int	main(void)
{
	dprintf(3, "Won't appear on stdout\n");
	if (dup2(STDOUT_FILENO, 3) == -1)
		return (1);
	dprintf(3, "Will appear on stdout\n");
	close(3);
	return (0);
}
