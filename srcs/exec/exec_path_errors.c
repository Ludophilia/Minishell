/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 23:05:33 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/07 23:39:43 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exc_err_pathg(char *path, char *strerr, t_core *core)
{
	ft_eprintf(ERR_PTH, path, strerr);
	core->exit = EX_CNFD;
	return (1);
}

int	exc_err_path(int access_test, char *path, t_core *core)
{
	ft_eprintf(ERR_PTH, path, strerror(errno));
	if (access_test && (errno == EACCES || errno == EISDIR))
		core->exit = EX_CNEX;
	else
		core->exit = EX_CNFD;
	return (1);
}

int	exc_err_cmd(char *path, t_core *core)
{
	ft_eprintf(ERR_CMD, path);
	core->exit = EX_CNFD;
	return (1);
}

int	exc_err_ecmd(char c, t_core *core)
{
	ft_eprintf(ERR_ECMD, c);
	core->exit = EX_CNFD;
	return (1);
}
