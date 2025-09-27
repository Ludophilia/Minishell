/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:04:49 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/27 23:32:34 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	fmgr_closeall(int from_id, int reverse, t_pgb *prgs)
// {
// 	t_pgb	prg;

// 	if (access(HRDC_NAME, F_OK) == 0)
// 		unlink(HRDC_NAME);
// 	while ((reverse && from_id >= 0) || (!reverse && prgs[from_id].cmd))
// 	{
// 		prg = prgs[from_id];
// 		if ((from_id == 0 && fmgr_close(prg.in, 0) == -1)
// 			|| fmgr_close(prg.out, 0) == -1
// 			|| fmgr_close(prg.out, 1) == -1)
// 			return (-1);
// 		if (reverse)
// 			from_id--;
// 		else
// 			++from_id;
// 	}
// 	return (1);
// }

// #################################################



int	fmgr_set_pipe(int pos, int pmax, t_cmd *cmd)
{
	t_cmd	*cmd;

	if (pmax == 0)
		return (0);
	if (pos != pmax && fmgr_pipe(cmd->ofds) == -1)
		return (-1); // close_all fds that has been opened so far ??
	if (pos != 0)
	{
		cmd->ifds[0] = (cmd - 1)->ofds[0];
		cmd->ifds[1] = (cmd - 1)->ofds[1];
	}
	return (0);
}

// ####################################################################

// int	fmgr_clopip(int fds[2])
// {
// 	if (fmgr_close(0, fds) == -1 || fmgr_close(1, fds) == -1)
// 		return (-1);
// 	return (0);
// }


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
	int		opn;
	int		fds[2];
	char	*lim;
		
	if (fmgr_pipe(fds) == -1)
		return (-1); // Error management?
	opn = 1;
	lim = red->word;
	while (opn)
		if (hrm_get_user_input(lim, ft_strlen(lim), &opn, fds) == -1)
			return (fmgr_close(0, fds), fmgr_close(1, fds), -1);
	if (fmgr_close(1, fds) == -1
		|| (ifds[0] != 0 && fmgr_close(0, ifds) == -1))
		return (fmgr_close(0, fds), -1);
	ifds[0] = fds[0];
	return (0);
}

int	fmgr_set_ired(int *ifds, t_red *red)
{
	int	fd;

	fd = fmgr_open(red->word, O_RDONLY, FL_PRMS);
	if (fd == -1)
		return (-1); // Cleanup?
	if (ifds[0] != 0 && fmgr_close(0, ifds) == -1)
		return (fmgr_close(0, &fd), -1);
	ifds[0] = fd;
	return (0);
}

int	fmgr_set_ireds(t_cmd *cmd)
{
	t_red	*ireds;
	int		j;

	j = -1;
	ireds = cmd->ireds;
	while (ireds[++j].type != TOK_EOL)
	{
		if (ireds[j].type == TOK_IRED
			&& fmgr_set_ired(cmd->ifds, ireds + j) == -1)
			return (-1); // Error management ??
		else if (ireds[j].type == TOK_IRED_HD
			&& fmgr_set_hdocs(cmd->ifds, ireds + j) == -1)
			return (-1); // Error management ??
	}
	return (0);
}

// #########################################################

// 28/09

// Open every type of redirections.

//	= [o] Open the '>'
//	= [o] Open the '>>'

int	fmgr_set_oreds(t_cmd *cmd)
{
	t_red	*oreds;
	int		j;

	j = -1;
	oreds = cmd->oreds;
	while (oreds[++j].type != TOK_EOL)
	{
		// bruv.
		continue ;
	}
	return (0);
}

int	fmgr_set_reds(t_core *core)
{
	int		i;
	t_cmd	*cmd;

	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		if (fmgr_set_pipe(i, core->cmd_pmax, cmd) == -1)
			return (-1);
		if (fmgr_set_ireds(cmd) == -1
			|| fmgr_set_oreds(cmd) == -1)
			continue ; // Error management? An error does not mean the end 
			// of the pipeline
		cmd->xready = true;
	}
	return (0);
}
