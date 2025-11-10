/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr_reds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 21:29:52 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/18 22:24:25 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fmgr_set_pipe(int pos, int pmax, t_cmd *cmd)
{
	int	fds[2];

	if (pos == pmax)
		return (0);
	if (fmgr_pipe(fds) == -1)
		return (-1);
	cmd->ofd = fds[1];
	(cmd + 1)->ifd = fds[0];
	return (0);
}

int	fmgr_set_red(int *xfd, int openflags, t_red *red, t_core *core)
{
	int	fd;

	fd = fmgr_open(red->word, openflags, FL_PRMS);
	if (fd == -1)
	{
		core->exit = 1;
		return (-1);
	}
	if (*xfd > 2 && fmgr_close(xfd) == -1)
		return (fmgr_close(&fd), -1);
	core->exit = 0;
	*xfd = fd;
	return (0);
}
