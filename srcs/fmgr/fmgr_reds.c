/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr_reds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 21:29:52 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/28 23:34:22 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fmgr_get_user_input(char *lim, int len, int *opn, int *fds)
{
	char	*uinp;

	if (write(1, "> ", 2) == -1)
		return (-1);
	uinp = get_next_line(0);
	if (uinp == NULL)
		return (-1);
	if (!ft_strncmp(uinp, lim, len) && uinp[len] == '\n')
		return ((*opn)--, free(uinp), 0);
	else if (write(fds[1], uinp, ft_strlen(uinp)) == -1)
		return (free(uinp), -1);
	return (free(uinp), 0);
}

int	fmgr_set_hdocs(int *ifds, t_red *red)
{
	int		opn;
	int		fds[2];
	char	*lim;
		
	if (fmgr_pipe(fds) == -1)
		return (-1);
	opn = true;
	lim = red->word;
	while (opn)
		if (fmgr_get_user_input(lim, ft_strlen(lim), &opn, fds) == -1)
			return (fmgr_close(0, fds), fmgr_close(1, fds), -1);
	if (fmgr_close(1, fds) == -1
		|| (ifds[0] > 2 && fmgr_close(0, ifds) == -1))
		return (fmgr_close(0, fds), -1);
	ifds[0] = fds[0];
	return (0);
}

int	fmgr_set_pipe(int pos, int pmax, t_cmd *cmd)
{
	if (pmax == 0)
		return (0);
	if (pos != pmax && fmgr_pipe(cmd->ofds) == -1)
		return (-1);
	if (pos != 0)
		cmd->ifds[0] = (cmd - 1)->ofds[0];
	return (0);
}

int	fmgr_set_red(int pos, int *fds, int openflags, t_red *red)
{
	int	fd;

	fd = fmgr_open(red->word, openflags, FL_PRMS);
	if (fd == -1)
		return (-1);
	if (fds[pos] > 2 && fmgr_close(0, fds) == -1)
		return (fmgr_close(0, &fd), -1);
	fds[pos] = fd;
	return (0);
}
