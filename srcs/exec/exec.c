/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/30 17:56:06 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exc_wait_cmds(int i)
{
	int	wstat;
	int	cfails;

	cfails = 0;
	while (--i >= 0)
		if (waitpid(-1, &wstat, 0) == -1 || WIFEXITED(wstat) == EXIT_FAILURE)
			++cfails;
	if (cfails != 0)
		return (-1);
	return (0);
}

int	exc_is_builtin(char *arg)
{
	char	**builtins;
	int		j;

	builtins = (char *[8]){"echo", "cd", "pwd", "export",
		"unset", "env", "exit", 0};
	j = -1;
	while (builtins[++j])	
		if (!ft_strncmp(*arg, builtins[j], ft_strlen(builtins[j])))
			return (j);
	return (0);
}

// int	exc_builtins(int id, char **argv, char **envp)
// {
// 	int (**arr)();
	
// 	arr = (int (*[8])()){, NULL};
	
// 	arr[id]();
// }

static int	exc_exec_cmd(t_cmd *cmd, char **envp)
{
	int	is_built;

	if (cmd->argv == NULL
		|| (*cmd->argv == NULL && ft_eprintf(ERR_CMD, NULL))
		|| dup2(cmd->ifd, 0) == -1
		|| dup2(cmd->ofd, 1) == -1
		|| fmgr_close(&cmd->ifd) == -1
		|| fmgr_close(&cmd->ofd) == -1)
	{
		close(cmd->ifd);
		close(cmd->ofd);
		return (-1);
	}
	is_built = exc_is_builtin(*cmd->argv);
	// if (is_built && exc_builtins(cmd->argv, envp) == -1) // arrays to pointer to funct?
	// 	return (-1);
	if (!is_built && (exc_check_path(cmd->argv, envp) == -1
		|| execve(*cmd->argv, cmd->argv, envp) == -1))
		return (-1);
	return (0);
}

int	exc_exec_cmds(t_core *core)
{
	pid_t	pid;
	int		i;

	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		pid = fork();
		if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
			return (exc_wait_cmds(i), -1);
		if (pid == 0 && exc_exec_cmd(core->cmds + i, core->envp) == -1)
			exit(EXIT_FAILURE);
	}
	if (exc_wait_cmds(i) == -1)
		return (-1); // close fds??
	return (0);
}
