/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr_hdocs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 21:29:52 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/28 21:30:30 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hrm_get_user_input(char *lim, int len, int *opn, int *fds)
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
	bool	opn;
	int		fds[2];
	char	*lim;
		
	if (fmgr_pipe(fds) == -1)
		return (-1); // Error management?
	opn = true;
	lim = red->word;
	while (opn)
		if (hrm_get_user_input(lim, ft_strlen(lim), &opn, fds) == -1)
			return (fmgr_close(0, fds), fmgr_close(1, fds), -1);
	if (fmgr_close(1, fds) == -1
		|| (ifds[0] > 2 && fmgr_close(0, ifds) == -1))
		return (fmgr_close(0, fds), -1);
	ifds[0] = fds[0];
	return (0);
}
