/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:17:11 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/14 13:46:36 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fmgr_close_extra_pipes(int ifd, int ofd, t_core *core)
{
	int		i;
	int		*pipe;
	int		fails;

	fails = 0;
	i = -1;
	while (core->stash[++i])
	{
		pipe = (core->stash[i])->content;
		ft_eprintf("\t\t[%i] processing (%i, %i) against (%i, %i)\n",
			getpid(), pipe[0], pipe[1], ifd, ofd);

		if (pipe[0] != ifd && pipe[0] != ofd)
		{
			ft_eprintf("\t\t\t[%i] pipe[0] -> %i at %p doesn't match with"
				" (%i, %i). Removing...\n", getpid(), pipe[0], pipe + 0, ifd, ofd);

			if (fmgr_close(pipe) == -1)
				fails++;
			// 14/01 - Each time you remove a reference in a subshell or cmdshell
			// you remove it here as well, so you don't end up removing it twice
			// and encountering a "bad file descriptor" error.


			// pipe[0] = -1;
			
		}
		if (pipe[1] != ifd && pipe[1] != ofd)
		{
			ft_eprintf("\t\t\t[%i] pipe[1] -> %i at %p doesn't match with"
				" (%i, %i). Removing...\n", getpid(), pipe[1], pipe + 1, ifd, ofd);
			if (fmgr_close(pipe + 1) == -1)
				fails++;

			// pipe[1] = -1;

		}
		// 	|| (pipe[1] != ifd
		// 		&& pipe[1] != ofd
		// 		&& fmgr_close(pipe + 1) == -1))
		// 	fails++;
			
		// if ((pipe[0] != ifd
		// 		&& pipe[0] != ofd
		// 		&& fmgr_close(pipe) == -1)
		// 	|| (pipe[1] != ifd
		// 		&& pipe[1] != ofd
		// 		&& fmgr_close(pipe + 1) == -1))
		// 	fails++;
		// core->stash[i] = NULL; 
	}
	if (fails && printf("\t[%i] fmgr_close_extra_pipes failure\n", getpid()))
		return (-1);
	return (0);
}
