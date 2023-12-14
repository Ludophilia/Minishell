/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:16:33 by jgermany          #+#    #+#             */
/*   Updated: 2023/06/27 13:21:51 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	**free_stash(char **stash)
{
	if (!*stash)
	{
		free(stash);
		return (0);
	}
	return (stash);
}

static char	**write_stash(char **stash, ssize_t bytesread, char *buffer)
{
	char	*old_stash;
	char	*new_stash;
	size_t	size;
	size_t	i;

	old_stash = *stash;
	size = bytesread;
	if (old_stash)
		size += ft_strlen(old_stash);
	new_stash = ft_calloc(size + 1, sizeof(char));
	if (!new_stash)
		return (0);
	i = 0;
	if (old_stash)
		while (i < (size - bytesread))
			new_stash[i++] = *old_stash++;
	while (i < size)
		new_stash[i++] = *buffer++;
	if (*stash)
		free(*stash);
	*stash = new_stash;
	return (stash);
}

ssize_t	update_stash(int fd, char **stash)
{
	ssize_t		bytesread;
	char		*buffer;

	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (-1);
	bytesread = read(fd, buffer, BUFFER_SIZE);
	if (bytesread > 0)
		stash = write_stash(stash, bytesread, buffer);
	free(buffer);
	return (bytesread);
}

char	*extract_line(char **stash)
{
	char	*old_stash;
	char	*line;
	int		newline_pos;
	size_t	stash_size;

	if (stash == NULL)
		newline_pos = -1;
	else
		newline_pos = ft_strchr(*stash, '\n') - *stash;
	stash_size = ft_strlen(*stash) - (newline_pos + 1);
	if (newline_pos >= 0 && stash_size)
	{
		old_stash = *stash;
		line = ft_substr(old_stash, 0, newline_pos + 1);
		*stash = ft_substr(old_stash, newline_pos + 1, stash_size);
		free(old_stash);
	}
	else
	{
		line = *stash;
		*stash = 0;
	}
	return (line);
}
