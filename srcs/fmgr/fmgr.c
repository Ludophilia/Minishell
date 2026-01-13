/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:04:49 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/13 20:38:15 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fmgr_set_red(int *xfd, t_red *red, t_core *core)
{
	int	fd;
	int openflags;

	openflags = FL_IRED;
	if (red->type == TOK_ORED)
		openflags = FL_ORED;
	else if (red->type == TOK_ORED_AP)
		openflags = FL_ORED_AP;



	fd = fmgr_open(red->word, openflags, FL_PRMS);
	if (fd == -1)
	{
		// core->exit = 1;
		return (-1);
	}
	if (*xfd > 2)
	{
		// 13/01. If *xfd is in the list of pipes;
		int		*pipes;
		int		i;
		
		i = -1;
		while (core->stash[++i])
		{
			pipes = core->stash[i]->content;
			if (pipes[0] == *xfd)
				pipes[0] = -1;
			else if (pipes[1] == *xfd)
				pipes[1] = -1;
		}
		
		if (fmgr_close(xfd) == -1)
			return (fmgr_close(&fd), -1);
	}
	core->exit = 0;
	*xfd = fd;
	return (0);
}

static int	fmgr_set_xfds(t_cmd *cmd, t_core *core)
{
	t_red	*red;
	int		j;

	j = -1;
	red = cmd->reds + ++j;
	while (red->type != TOK_EOL)
	{
		if ((red->type == TOK_IRED
				&& fmgr_set_red(&cmd->ifd, red, core) == -1)
			|| (red->type == TOK_ORED
				&& fmgr_set_red(&cmd->ofd, red, core) == -1)
			|| (red->type == TOK_IRED_HD
				&& fmgr_set_hdocs(&cmd->ifd, red, core) == -1)
			|| (red->type == TOK_ORED_AP
				&& fmgr_set_red(&cmd->ofd, red, core) == -1))
			return (-1);
		red = cmd->reds + ++j;
	}
	return (0);
}

// 13/01 - Merge the two competing logic together

// 12/01.
// - It's those "close_extra_pipes" functions that create dysfuctions
// In the beginning: ifd: 0, ofd: 4. 
// - fmgr_process_reds closed ofd: 4 to replace it with ofd: 5.
// - when fmgr_close_extra_pipes in tries to close 4 that is already closed
int	fmgr_process_reds(int *ifd, int *ofd, t_cmd *cmd, t_core *core)
{
	ft_eprintf("\t[%i] About to close extra pipes in subshell except (%i, %i)\n",
			getpid(), *ifd, *ofd);
	if (fmgr_close_extra_pipes(*ifd, *ofd, core) == -1)
		return (core->exit = EX_F, -1);
	cmd->ifd = *ifd;
	cmd->ofd = *ofd;
	if (fmgr_set_xfds(cmd, core) == -1)
		return (core->exit = EX_F, -1);
	*ifd = cmd->ifd;
	*ofd = cmd->ofd;
	return (0);
}
