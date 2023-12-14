/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:20:29 by jgermany          #+#    #+#             */
/*   Updated: 2023/06/27 15:00:03 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char		**stash;
	char			*line;
	ssize_t			bytesread;

	if (!stash)
		stash = ft_calloc(1, sizeof(char *));
	if (!stash || BUFFER_SIZE < 1 || fd < 0)
	{
		stash = free_stash(stash);
		return (0);
	}
	bytesread = update_stash(fd, stash);
	while (bytesread > 0 && ft_strchr(*stash, '\n') == NULL)
		bytesread = update_stash(fd, stash);
	if ((bytesread == 0 && !*stash) || bytesread == -1)
	{
		stash = free_stash(stash);
		return (0);
	}
	line = extract_line(stash);
	stash = free_stash(stash);
	return (line);
}
