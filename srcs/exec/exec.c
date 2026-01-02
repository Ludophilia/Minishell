/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/02 21:04:45 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exc_process_reds(int *ifd, int *ofd, t_astn *root, t_core *core)
{
	t_cmd	*cmd;

	cmd = root->content;
	cmd->ifd = *ifd;
	cmd->ofd = *ofd;
	if (fmgr_set_xfds(cmd, core) == -1)
		return (-1);
	*ifd = cmd->ifd;
	*ofd = cmd->ofd;
	return (0);
}

// #########################################################################

int	exc_exec_ao(int ifd, int ofd, t_astn *root, t_core *core)
{
	int	status;
	int	op;

	op = root->op;
	status = exc_exec_ast(ifd, ofd, root->left, core);
	if ((op == TOK_AND && status == 0) || (op == TOK_OR && status > 0))
		status = exc_exec_ast(ifd, ofd, root->right, core);
	return (status);
}

int	exc_exec_pipe(int ifd, int ofd, t_astn *root, t_core *core)
{
	int	pipe[2];
	int	status;

	if (fmgr_pipe(pipe) == -1)
		return (-1);
	status = exc_exec_ast(pipe[1], ofd, root->left, core);
	if (status != -1)
		status = exc_exec_ast(ifd, pipe[0], root->right, core);
	if (close(pipe[0]) == -1 || close(pipe[1]) == -1)
		return (-1);
	return (status);
}

int	exc_exec_sub(int ifd, int ofd, t_astn *root, t_core *core)
{
	pid_t	pid;
	int		wstatus;
	int		status;

	pid = fork();
	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (pid == 0)
	{
		if (root->content && exc_process_reds(&ifd, &ofd, root, core) == -1)
			return (-1);
		if ((ifd > 2 && dup2(ifd, STDIN_FILENO) == -1)
			|| (ofd > 2 && dup2(ofd, STDOUT_FILENO) == -1))
			return (-1);
		// (sig_init_child() == -1 && utl_exit(EX_FAIL, core))
		// return (-1);
		status = exc_exec_ast(-1, -1, root->left, core); // 31/12: not the same core...
		if (status == -1 && utl_cleanup(core->flags, 0, core))
			utl_exit(EX_FAIL, core); // 2/01 - Faulty?
		utl_cleanup(core->flags, 0, core);
		utl_exit(EX_SUCC, core); // 31/12 - And this as well.
	}
	// 31/12 - command status should be transferred.
	if (pid > 0 && (waitpid(pid, &wstatus, 0) == -1 || wstatus == 1))
		return (-1); // core->exit = ???
	return (status);
}

// ###################################################################

static int	exc_exec_prg(t_cmd *cmd, t_core *core)
{
	int	chk_val;

	if (fmgr_dup2(cmd->ifd, 0) == -1 || fmgr_dup2(cmd->ofd, 1) == -1
		|| utl_cleanup(FLG_REDS, core) != 1
		|| env_get_envp(core->env, core) == NULL)
		return (-1);
	chk_val = exc_check_path(cmd->argv, core->envp);
	if (chk_val == -2 && utl_exit(126, core))
		return (0);
	if (chk_val == -1)
		return (-1);
	if (chk_val == 0 && utl_exit(EX_CNFD, core))
		return (0);
	if (chk_val && execve(*cmd->argv, cmd->argv, core->envp) == -1)
		return (-1);
	return (0);
}

// ###################################################################

int	exc_exec_cmd(int ifd, int ofd, t_astn *root, t_core *core)
{
	t_cmd	*cmd;
	pid_t	pid;

	if (exc_process_reds(&ifd, &ofd, root, core) == -1)
		return (-1);
	cmd = root->content;

	if ((cmd->argv == NULL) 
		|| (*cmd->argv == NULL)
		|| (**cmd->argv == 0 && ft_eprintf(ERR_ECMD, **cmd->argv))
		|| (core->cmds == 1 && exc_if_builtin(cmd, core)))
		return (0); // 31/12 - 0? Is that correct?

	pid = fork();
	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (pid > 0)
	{
		cmd->pid = pid;
		if (exc_wait_cmds(core) == -1)
			return (-1);
		return (0);
	}

	if (pid == 0)
	{
		// 2/01 - Builtin
		if (exc_if_builtin(cmd, core))
			utl_exit(core->exit, core);
		// 2/01 - Or program in the filesystem
		if (sig_init_child() == -1
			|| exc_exec_prg(cmd, core) == -1)
			utl_exit(EX_FAIL, core);
	}
	return (0);
}

// 29/12 - That's a draft. So there's shit in it. ifd and ofd may not be the right call here. 
// 31/12 - Which status? core->exit for the exit status from commands line?
int	exc_exec_ast(int ifd, int ofd, t_astn *root, t_core *core)
{
	int	status; // 31/12: status? What's for?

	// 30/12 - How is -1 returned to the caller?
	if (root->type == AST_AO)
		status = exc_exec_ao(ifd, ofd, root, core);
	if (root->type == AST_PI)
		status = exc_exec_pipe(ifd, ofd, root, core);
	if (root->type == AST_SUB) 
		status = exc_exec_sub(ifd, ofd, root, core);
	if (root->type == AST_CMD)
		status = exc_exec_cmd(ifd, ofd, root, core);
	return (status);
}

