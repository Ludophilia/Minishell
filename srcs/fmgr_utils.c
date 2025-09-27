/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:05:19 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/27 17:29:15 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fmgr_access(char *path, int type)
{
	if (access(path, type) == -1)
	{
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

int	fmgr_pipe(int fds[2])
{
	if (pipe(fds) == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	return (0);
}

int	fmgr_close(int pos, int *fds)
{
	// if (prg_fds[end] <= 2) // ???
	// 	return (0);
	if (close(fds[pos]) == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	fds[pos] = 0; // 0 or -1 ?
	return (0);
}