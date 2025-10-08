/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttyslot.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 19:33:47 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/10 14:52:50 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	int slot;
	
	slot = ttyslot();
	if (slot == 0)
	{
		dprintf(2, "Error: Unable to determine terminal slot.\n");
		return (1);
	}
	else if (slot > 0)
		dprintf(2, "Current user's terminal slot in utmp file: %d\n", slot);
	return (0);
}
