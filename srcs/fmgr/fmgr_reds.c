/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr_reds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 21:29:52 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/29 16:53:12 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fmgr_get_user_input(int *opn, int *fds, t_red *red)
{
	char	*uinp;
	char	*lim;
	int		len;

	if (write(1, "> ", 2) == -1)
		return (-1);
	uinp = get_next_line(0);
	if (uinp == NULL)
		return (-1);
	lim = red->word;
	len = ft_strlen(lim);
	if (!ft_strncmp(uinp, lim, len) && uinp[len] == '\n')
		return ((*opn)--, free(uinp), 0);
	else if (write(fds[1], uinp, ft_strlen(uinp)) == -1)
		return (free(uinp), -1);
	return (free(uinp), 0);
}

int	fmgr_set_hdocs(int *ifd, t_red *red)
{
	int		opn;
	int		fds[2];

	if (ifd == 0)
		return (0);
	if (fmgr_pipe(fds) == -1)
		return (-1);
	opn = true;
	while (opn)
		if (fmgr_get_user_input(&opn, fds, red) == -1)
			return (close(fds[0]), close(fds[1]), -1);
	if (fmgr_close(fds + 1) == -1
		|| (*ifd > 2 && fmgr_close(ifd) == -1))
		return (close(fds[0]), -1);
	*ifd = fds[0];
	return (0);
}

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

int	fmgr_set_red(int *xfd, int openflags, t_red *red)
{
	int	fd;

	fd = fmgr_open(red->word, openflags, FL_PRMS);
	if (fd == -1)
		return (-1);
	if (*xfd > 2 && fmgr_close(xfd) == -1)
		return (fmgr_close(&fd), -1);
	*xfd = fd;
	return (0);
}
