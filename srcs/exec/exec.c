/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/10 15:50:54 by jegerman         ###   ########.fr       */
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

	// printf("(exc_exec_aon) core->exit: %i\n", core->exit);

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
	if (exp_consume_redtoks(cmd, core) == -1
		|| exp_consume_wtoks(cmd, core) == -1)
		return (-1);
	if (core->cmds == 1 && exc_if_builtin(cmd, core))
		return (0);
	pid = fork();
	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (pid > 0 && exc_wait_pid(pid, core) == -1)
		return (-1);
	if (pid == 0)
	{	
		status = exc_exec_scmd(ifd, ofd, root, core);
		utl_exit(status, core);
	}
	return (0);
}

// 8/01 - PROBLEMS.

// [ ] (echo a && echo b && echo c) > test | nl 
//		-> bad file descriptor (why?)

// [x] (exit 42) || echo exit code: $?
//		-> substitution happens at the wrong level

// [ ] export ECOLE=42 && (echo $ECOLE)
//		 -> There's a problem with ENV inheritance.

// [ ] echo 123456789 > a > b > c > d
//		-> Redirections do not work.

// [ ]- Tudieu, ca LEAKE la!!
//		-> Ou?

int	exc_exec_subn(int ifd, int ofd, t_astn *root, t_core *core)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (pid > 0 && exc_wait_pid(pid, core) == -1)
		return (-1);
	if (pid == 0)
	{

		if (exc_close_pipes(ifd, ofd, core) == -1)
			return (EX_F);
		if (root->content && exc_process_reds(&ifd, &ofd, root, core) == -1)
			return (EX_F);
		if (sig_init_child() == -1
			|| fmgr_dup2(ifd, 0) == -1
			|| fmgr_dup2(ofd, 1) == -1) // || env_get_envp(core->env, core) == NULL
			return (EX_F);

		// dprintf(2, "(exc_exec_subn) ifd -> %i; ofd -> %i\n", ifd, ofd);

		if (exc_exec_ast(0, 1, root->left, core) == -1)
			utl_exit(EX_F, core);
		utl_exit(core->exit, core);

	}
	// printf("(end exc_exec_subn) core->exit -> %i\n", core->exit);
	return (0);
}

// 7/01 - DONT FORGET NORMINETTE
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

