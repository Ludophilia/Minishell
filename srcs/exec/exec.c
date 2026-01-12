/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2026/01/12 17:56:44 by jegerman         ###   ########.fr       */
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
		if (exc_process_reds(&ifd, &ofd, cmd, core) == -1)
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
		ft_eprintf("[%i] IN CMD with ifd: %i, ofd: %i\n",
			getpid(), ifd, ofd); //
		status = exc_exec_scmd(ifd, ofd, root, core);
		utl_exit(status, core);
	}
	return (0);
}

// 8/01 - PROBLEMS.

// [o] (echo a) | nl
// [o] (echo a && echo b && echo c) | nl
//		-> (still works, but opened pipe is found in the shell when exited)

// [o] (echo a) > test
// [o] (echo a && echo b && echo c) > test

// [ ] (echo a) > test | nl
// [ ] (echo a && echo b && echo c) > test | nl
//		-> bad file descriptor (why?, still not working)


// [x] (echo a && echo b && echo c) (works)

// [x] echo 123456789 > a > b > c
//		-> Redirections do not work.

// [x] (exit 42) || echo exit code: $?
//		-> substitution happens at the wrong level

// For Nizar
// [ ] export ECOLE=42 && (echo $ECOLE)
//		 -> There's a problem with ENV inheritance.

int	exc_exec_subshell(int ifd, int ofd, t_astn *root, t_core *core)
{
	t_cmd	*cmd;

	cmd = root->content;

	ft_eprintf("[%i] IN SUBSHELL... ifd: %i, ofd: %i\n",
		getpid(), ifd, ofd);

	// 12/01: Not in the right spot... ? Later?

	// if (exc_close_extra_pipes(ifd, ofd, core) == -1)
	// 	return (EX_F);


	if (root->content
		&& (exp_cnsm_rtoks(cmd, core) == -1
			|| exc_process_reds(&ifd, &ofd, cmd, core) == -1)) // 12/01 - Will change
		return (EX_F);



	// 12/01.
	// - It's those "close_extra_pipes" functions that create dysfuctions
	// In the beginning: ifd: 0, ofd: 4. 
	// - exc_process_reds closed ofd: 4 to replace it with ofd: 5.
	// - when exc_close_extra_pipes tries to close 4 that is already closed

	// NOW...
	// What would be your solution
	if (exc_close_extra_pipes(ifd, ofd, core) == -1)
		return (EX_F);


	if (root->content)
	{
		if (sig_init_child() == -1
			|| fmgr_dup2(cmd->ifd, 0) == -1
			|| fmgr_dup2(cmd->ofd, 1) == -1) // || env_get_envp(core->env, core) == NULL
			return (EX_F);
	}
	else
	{
		if (sig_init_child() == -1
			|| fmgr_dup2(ifd, 0) == -1
			|| fmgr_dup2(ofd, 1) == -1) // || env_get_envp(core->env, core) == NULL
			return (EX_F);
	}

	// 12/01: We don't really use ifd and ofd after that.
	// AT LEAST up to now.
	if (exc_exec_ast(0, 1, root->left, core) == -1)
		return (EX_F);

	// ft_eprintf("[%i] ifd: %i ; ofd: %i\n", getpid(), ifd, ofd);
	
	// 12/01: It's because we don't use ifd and ofd in exc_exec_ast above...
	if (root->content == NULL && (fmgr_close(&ifd) == -1 || fmgr_close(&ofd) == -1)) // || env_get_envp(core->env, core) == NULL
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

