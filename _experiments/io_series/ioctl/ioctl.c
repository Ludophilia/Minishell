/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ioctl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 15:07:32 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/10 16:24:30 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>

int	main_get_terminal_size(struct winsize *win_size)
{
	if (ioctl(0, TIOCGWINSZ, win_size) == -1)
		return (-1);
	printf("Terminal window size: rows=%d, cols=%d\n",
		win_size->ws_row, win_size->ws_col);
	return (0);
}

int	main_set_terminal_size(uint16_t col, uint16_t row, struct winsize *win_size)
{	
	win_size->ws_col = col;
	win_size->ws_row = row;
	if (ioctl(0, TIOCSWINSZ, win_size) == -1)
		return (-1);
	printf("Terminal window size: rows=%d, cols=%d\n",
		win_size->ws_row, win_size->ws_col);
	return (0);
}

int	main(void)
{
	struct winsize	win_size;
	
	if (main_get_terminal_size(&win_size) == -1)
		return (-1);
	return (0);
}
