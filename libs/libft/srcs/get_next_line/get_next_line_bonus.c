/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:51:33 by jegerman          #+#    #+#             */
/*   Updated: 2025/03/08 17:16:28 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static ssize_t	get_char_pos(const char *s, int c)
{
	ssize_t	len;
	ssize_t	i;

	len = ft_strlen(s);
	i = 0;
	while (i < len)
	{
		if (s[i] == (char)c)
			return (i);
		++i;
	}
	return (-1);
}

static int	swap_stash(char **stash, char *candidate)
{
	char	*old_stash;

	old_stash = *stash;
	if (candidate == NULL)
	{
		*stash = NULL;
		free(old_stash);
		return (-1);
	}
	*stash = candidate;
	free(old_stash);
	return (0);
}

static ssize_t	update_stash(int fd, char *buffer, char **stash)
{
	ssize_t		bytesread;
	char		*tmp_stsh;
	int			i;

	i = -1;
	while (++i < BUFFER_SIZE + 1)
		buffer[i] = 0;
	bytesread = read(fd, buffer, BUFFER_SIZE);
	if (bytesread == -1 || bytesread == 0)
		return (bytesread);
	if (*stash == NULL)
	{
		*stash = ft_strdup(buffer);
		if (*stash == NULL)
			return (-1);
		return (bytesread);
	}
	tmp_stsh = ft_strjoin(*stash, buffer);
	if (swap_stash(stash, tmp_stsh) == -1)
		return (-1);
	return (bytesread);
}

static char	*extract_line(char **stash)
{
	ssize_t	nl_pos;
	char	*line;
	char	*tmp_stsh;

	nl_pos = get_char_pos(*stash, '\n');
	if (nl_pos == -1 || nl_pos + 1 == (ssize_t)ft_strlen(*stash))
	{
		line = *stash;
		*stash = NULL;
		return (line);
	}
	line = ft_substr(*stash, 0, nl_pos + 1);
	if (line == NULL)
	{
		free(stash);
		*stash = NULL;
		return (NULL);
	}
	tmp_stsh = ft_substr(*stash, nl_pos + 1, ft_strlen(*stash) \
		- (nl_pos + 1));
	if (swap_stash(stash, tmp_stsh) == -1)
		return (NULL);
	return (line);
}

char	*get_next_line(int fd)
{
	static char		*stash[65535];
	char			*line;
	ssize_t			bytesread;
	char			*buffer;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buffer == NULL)
		return (NULL);
	bytesread = update_stash(fd, buffer, stash + fd);
	while (bytesread > 0 && get_char_pos(stash[fd], '\n') == -1)
		bytesread = update_stash(fd, buffer, stash + fd);
	free(buffer);
	if (bytesread == -1)
	{
		if (stash[fd])
			free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	if (stash[fd] == NULL)
		return (NULL);
	line = extract_line(stash + fd);
	return (line);
}
