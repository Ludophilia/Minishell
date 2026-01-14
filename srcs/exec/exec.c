/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/14 12:47:06 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exc_exec_aon(int ifd, int ofd, t_astn *root, t_core *core)
{
	int	op;

	op = root->op;
	if (exc_exec_ast(ifd, ofd, root->left, core) == -1)
		return (-1);
	if ((op == TOK_AND && core->exit > 0)
		|| (op == TOK_OR && core->exit == 0))
		return (0);
	if (exc_exec_ast(ifd, ofd, root->right, core) == -1)
		return (-1);
	return (0);
}

static int	exc_exec_pipn(int ifd, int ofd, t_astn *root, t_core *core)
{
	int		pipe[2];
	int		i;

	if (fmgr_pipe(pipe) == -1)
		return (-1);
	root->content = pipe;
	i = 0;
	while (core->stash[i])
		i++;
	core->stash[i] = root;
	if (exc_exec_ast(ifd, pipe[1], root->left, core) == -1
		|| fmgr_close(pipe + 1) == -1
		|| exc_exec_ast(pipe[0], ofd, root->right, core) == -1
		|| fmgr_close(pipe) == -1)
	{
		core->stash[i] = 0;
		return (close(pipe[0]), close(pipe[1]), -1);
	}
	core->stash[i] = 0;
	return (0);
}

static int	exc_exec_cmdn(int ifd, int ofd, t_astn *root, t_core *core)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		status;

	cmd = root->content;
	if (exp_cnsm_rtoks(cmd, core) == -1 || exp_cnsm_wtoks(cmd, core) == -1)
		return (-1);
	if (core->cmds == 1 && exc_is_builtin(cmd, &status))
	{
		if (fmgr_process_reds(&ifd, &ofd, cmd, core) == -1)
			return (-1);
		core->exit = exc_exec_builtin(status, cmd, core);
		return (0);
	}
	pid = fork();
	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (pid > 0 && exc_wait_pid(pid, core) == -1)
		return (-1);
	if (pid == 0)
	{	
		ft_eprintf("[%i] IN CMD... ifd: %i, ofd: %i\n",
			getpid(), ifd, ofd); //
		status = exc_exec_scmd(ifd, ofd, root, core);
		utl_exit(status, core);
	}
	return (0); 
}

int	exc_exec_subshell(int ifd, int ofd, t_astn *root, t_core *core)
{
	t_cmd	*cmd;

	cmd = root->content;

	ft_eprintf("[%i] IN SUBSHELL... ifd: %i, ofd: %i\n",
		getpid(), ifd, ofd); //

	if (root->content && exp_cnsm_rtoks(cmd, core) == -1)
		return (EX_F);

	// 14/01 - Where are the closed pipes? Current configuration close them
	// ONLY if there are redirections...

	ft_eprintf("\t[%i] About to close extra pipes in subshell except (%i, %i)\n",
			getpid(), ifd, ofd);

	if (fmgr_close_extra_pipes(ifd, ofd, core) == -1)
		return (core->exit = EX_F, -1);
		
	if (root->content && fmgr_process_reds(&ifd, &ofd, cmd, core) == -1)
		return (core->exit);

	if (root->content)
	{
		if (sig_init_child() == -1
			|| fmgr_dup2(cmd->ifd, 0) == -1
			|| fmgr_dup2(cmd->ofd, 1) == -1 // || env_get_envp(core->env, core) == NULL
			|| fmgr_close(&cmd->ifd) == -1
			|| fmgr_close(&cmd->ofd) == -1)
			return (EX_F);
	}
	else
	{
		if (sig_init_child() == -1
			|| fmgr_dup2(ifd, 0) == -1
			|| fmgr_dup2(ofd, 1) == -1 // || env_get_envp(core->env, core) == NULL
			|| fmgr_close(&ifd) == -1
			|| fmgr_close(&ofd) == -1)
			return (EX_F);
	}

	if (exc_exec_ast(0, 1, root->left, core) == -1)
		return (EX_F);

	return (core->exit);
}

int	exc_exec_subn(int ifd, int ofd, t_astn *root, t_core *core)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (pid > 0 && exc_wait_pid(pid, core) == -1)
		return (-1);
	if (pid == 0)
	{
		status = exc_exec_subshell(ifd, ofd, root, core);
		utl_exit(status, core);
	}
	// printf("(end exc_exec_subn) core->exit -> %i\n", core->exit);
	return (0);
}

int	exc_exec_ast(int ifd, int ofd, t_astn *root, t_core *core)
{
	if ((root->type == AST_AO
			&& exc_exec_aon(ifd, ofd, root, core) == -1)
		|| (root->type == AST_PI
			&& exc_exec_pipn(ifd, ofd, root, core) == -1)
		|| (root->type == AST_SUB
			&& exc_exec_subn(ifd, ofd, root, core) == -1)
		|| (root->type == AST_CMD
			&& exc_exec_cmdn(ifd, ofd, root, core) == -1))
		return (-1);
	return (0);
}

