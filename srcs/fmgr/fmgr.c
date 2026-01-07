/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:04:49 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/07 23:39:04 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fmgr_get_user_input(int *opn, t_red *red)
{
	char	*uinp;
	char	*lim;

	uinp = readline("> ");
	if (uinp == NULL)
		return (-1);
	lim = red->word;
	if (!ft_strncmp(uinp, lim, ft_strlen(lim) + 1))
		return ((*opn)--, free(uinp), 0);
	if (write(2, uinp, ft_strlen(uinp)) == -1 || write(2, "\n", 1) == -1)
		return (free(uinp), -1);
	return (free(uinp), 0);
}

static int	fmgr_subsh_hdocs(int *pid, int *fds, t_red *red, t_core *core)
{
	int		opn;

	*pid = fork();
	if (*pid == -1)
		return (-1);
	if (*pid > 0)
		return (0);
	if (sig_init_child() == -1 || close(fds[0]) == -1
		|| dup2(fds[1], 2) == -1 || close(fds[1]) == -1)
	{
		(close(fds[0]), close(fds[1]));
		return (utl_exit(EX_F, core));
	}
	signal(SIGQUIT, SIG_IGN);
	opn = true;
	while (opn)
		if (fmgr_get_user_input(&opn, red) == -1)
			return (utl_exit(EX_F, core));
	return (utl_exit(0, core), 0);
}

static int	fmgr_set_hdocs(int *ifd, t_red *red, t_core *core)
{
	int		fds[2];
	int		pid;
	int		wstat;

	if (ifd == NULL)
		return (0);
	if (fmgr_pipe(fds) == -1)
		return (-1);
	if (fmgr_subsh_hdocs(&pid, fds, red, core) == -1
		|| waitpid(pid, &wstat, 0) == -1)
		return (close(fds[0]), close(fds[1]), -1);
	if (WIFEXITED(wstat))
		core->exit = WEXITSTATUS(wstat);
	else if (WIFSIGNALED(wstat))
		core->exit = 128 + WTERMSIG(wstat);
	if (fmgr_close(fds + 1) == -1 || fmgr_close(ifd) == -1)
		return (close(fds[0]), -1);
	*ifd = fds[0];
	return (0);
}

static int	fmgr_set_red(int *xfd, int openflags, t_red *red, t_core *core)
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

int	fmgr_set_xfds(t_cmd *cmd, t_core *core)
{
	t_red	*red;
	int		j;

	j = -1;
	red = cmd->reds + ++j;
	while (red->type != TOK_EOL)
	{
		if ((red->type == TOK_IRED
				&& fmgr_set_red(&cmd->ifd, FL_IRED, red, core) == -1)
			|| (red->type == TOK_ORED
				&& fmgr_set_red(&cmd->ofd, FL_ORED, red, core) == -1)
			|| (red->type == TOK_IRED_HD
				&& fmgr_set_hdocs(&cmd->ifd, red, core) == -1)
			|| (red->type == TOK_ORED_AP
				&& fmgr_set_red(&cmd->ofd, FL_ORED_AP, red, core) == -1))
			return (-1);
		red = cmd->reds + ++j;
	}
	return (0);
}
