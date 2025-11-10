/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:05:19 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/19 14:19:20 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fmgr_access(char *path, int type)
{
	if (access(path, type) == -1)
	{
		if (errno == EACCES || errno == EISDIR)
		{
			ft_eprintf(ERR_PTH, path, strerror(errno));
			return (-2);
		}
		ft_eprintf(ERR_PTH, path, strerror(errno));
		return (-1);
	}
	return (0);
}

int	fmgr_open(char *path, int openflags, mode_t openmode)
{
	int	fd;

	if (path == NULL)
		return (-1);
	fd = open(path, openflags, openmode);
	if (fd == -1 && ft_eprintf(ERR_PTH, path, strerror(errno)))
		return (-1);
	return (fd);
}

int	fmgr_dup2(int old_fd, int new_fd)
{
	if (old_fd <= 2)
		return (0);
	if (dup2(old_fd, new_fd) == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	return (0);
}

int	fmgr_pipe(int fds[2])
{
	if (pipe(fds) == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	return (0);
}

int	fmgr_close(int *xfd)
{
	if (xfd == NULL || *xfd <= 2)
		return (0);
	if (close(*xfd) == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	*xfd = 0;
	return (0);
}
