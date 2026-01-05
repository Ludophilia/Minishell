/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/05 02:11:35 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #########################################################################

// int	exc_exec_ao(int ifd, int ofd, t_astn *root, t_core *core)
// {
// 	int	op;

// 	op = root->op;
// 	if (exc_exec_ast(ifd, ofd, root->left, core) == -1)
// 		return (-1);
// 	if ((op == TOK_AND && core->exit != 0)
// 		|| (op == TOK_OR && core->exit == 0))
// 		return (0);
// 	if (exc_exec_ast(ifd, ofd, root->right, core) == -1)
// 		return (-1);
// 	return (0);
// }

// int	exc_exec_pipe(int ifd, int ofd, t_astn *root, t_core *core)
// {
// 	int	pipe[2];

// 	if (fmgr_pipe(pipe) == -1)
// 		return (-1);
// 	if (exc_exec_ast(pipe[1], ofd, root->left, core) == -1
// 		|| exc_exec_ast(ifd, pipe[0], root->right, core) == -1)
// 		return (-1);
// 	 // 3/01/25: Are you sure? It will be closed after no?
// 	if (close(pipe[0]) == -1 || close(pipe[1]) == -1)
// 		return (-1);
// 	return (0);
// }

// int	exc_exec_sub(int ifd, int ofd, t_astn *root, t_core *core)
// {
// 	pid_t	pid;
// 	int		wstatus;

// 	pid = fork();
// 	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
// 		return (-1);


// 	if (pid == 0)
// 	{
// 		if (root->content && exc_process_reds(&ifd, &ofd, root, core) == -1)
// 			return (-1);


// 		if (fmgr_dup2(ifd, 0) == -1
// 			|| fmgr_dup2(ofd, 1) == -1
// 			|| env_get_envp(core->env, core) == NULL // 3/01, New env?
// 			|| (sig_init_child() == -1 && utl_exit(EXIT_F, core)))
// 			return (-1);

// 		if (exc_exec_ast(-1, -1, root->left, core) == -1)
// 			utl_exit(EXIT_F, core); // 2/01 - What should be the exit code?
// 		utl_exit(EXIT_S, core); // 31/12 - And this as well.
// 	}


// 	// 31/12 - exit code should be transferred from child to parent.
// 	if (pid > 0 && (waitpid(pid, &wstatus, 0) == -1 || wstatus == 1))
// 		return (-1); // core->exit = ???
// 	return (0);
// }

// ###################################################################

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

static int	exc_exec_prg(t_cmd *cmd, t_core *core)
{
	int	status;

	if (fmgr_dup2(cmd->ifd, 0) == -1 || fmgr_dup2(cmd->ofd, 1) == -1
		|| env_get_envp(core->env, core) == NULL)
		return (-1);
	status = exc_check_path(cmd->argv, core->envp);
	if (status == -1)
		return (-1);
	if ((status == -2 && utl_exit(EXIT_CNE, core))
		|| (status == 0 && utl_exit(EXIT_CNF, core)))
		return (0);
	if (status == 1 && execve(*cmd->argv, cmd->argv, core->envp) == -1)
		return (-1);
	return (0);
}

int	exc_exec_cmd(int ifd, int ofd, t_astn *root, t_core *core)
{
	t_cmd	*cmd;
	pid_t	pid;

	cmd = root->content;
	
	// if (core->cmds == 1 && exc_if_builtin(cmd, core))
	// 	return (0);


	pid = fork();
	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (pid > 0 && exc_wait_cmds(pid, core) == -1)
		return (-1);



	if (pid == 0)
	{

		if (exc_process_reds(&ifd, &ofd, root, core) == -1)
			utl_exit(core->exit, core);
	
		if (*cmd->argv == NULL)
			utl_exit(EXIT_S, core);

		// if (exc_if_builtin(cmd, core))
		// 	utl_exit(core->exit, core);

		if (sig_init_child() == -1
			|| exc_exec_prg(cmd, core) == -1)
			utl_exit(EXIT_F, core);
		utl_exit(EXIT_S, core);
	}
	return (0);
}

// 4/01 - From simple to complex. Let's start with a simpler system to see
// if we're on the right path or not and THEN build from there...
int	exc_exec_ast(int ifd, int ofd, t_astn *root, t_core *core)
{
	// t_astt	type;

	// 29/12 - ifd and ofd may not be good enough here...
	// 4/01 - Next up. Add redirections... (ongoing)
	if ((root->type == AST_CMD
			&& exc_exec_cmd(ifd, ofd, root, core) == -1))
		return (-1);

	// if ((root->type == AST_AO
	// 		&& exc_exec_ao(ifd, ofd, root, core) == -1)
	// 	|| (root->type == AST_CMD
	// 		&& exc_exec_cmd(ifd, ofd, root, core) == -1))
	// 	return (-1);

	// if ((root->type == AST_AO
	// 		&& exc_exec_ao(ifd, ofd, root, core) == -1)
	// 	|| (root->type == AST_PI
	// 		&& exc_exec_pipe(ifd, ofd, root, core) == -1)
	// 	|| (root->type == AST_CMD
	// 		&& exc_exec_cmd(ifd, ofd, root, core) == -1))
	// 	return (-1);

	// if ((root->type == AST_AO
	// 		&& exc_exec_ao(ifd, ofd, root, core) == -1)
	// 	|| (root->type == AST_PI
	// 		&& exc_exec_pipe(ifd, ofd, root, core) == -1)
	// 	|| (root->type == AST_SUB
	// 		&& exc_exec_sub(ifd, ofd, root, core) == -1)
	// 	|| (root->type == AST_CMD
	// 		&& exc_exec_cmd(ifd, ofd, root, core) == -1))
	// 	return (-1);

	return (0);
}

