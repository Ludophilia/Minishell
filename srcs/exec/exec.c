/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:07:42 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/29 21:58:14 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	exc_exec_prg(t_cmd *cmd, t_core *core)
// {
// 	int	chk_val;

// 	if (fmgr_dup2(cmd->ifd, 0) == -1 || fmgr_dup2(cmd->ofd, 1) == -1
// 		|| utl_cleanup(FLG_REDS, core) != 1
// 		|| env_get_envp(core->env, core) == NULL)
// 		return (-1);
// 	chk_val = exc_check_path(cmd->argv, core->envp);
// 	if (chk_val == -2 && utl_exit(126, core))
// 		return (0);
// 	if (chk_val == -1)
// 		return (-1);
// 	if (chk_val == 0 && utl_exit(EX_CNFD, core))
// 		return (0);
// 	if (chk_val && execve(*cmd->argv, cmd->argv, core->envp) == -1)
// 		return (-1);
// 	return (0);
// }

// static int	exc_exec_cmd(t_cmd *cmd, t_core *core)
// {
// 	if (**cmd->argv == 0
// 		&& ft_eprintf(ERR_ECMD, **cmd->argv) && utl_exit(EX_CNFD, core))
// 		return (0);
// 	if (exc_if_builtin(cmd, core) && utl_exit(core->exit, core))
// 		return (0);
// 	if (exc_exec_prg(cmd, core) == -1 && utl_exit(EX_FAIL, core))
// 		return (-1);
// 	utl_exit(EX_FAIL, core);
// 	return (0);
// }

// static int	exc_init_subsh(int i, pid_t *pid, t_core *core)
// {
// 	t_cmd	*cmd;

// 	cmd = core->cmds + i;
// 	*pid = fork();
// 	if (*pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
// 		return (-1);
// 	if (*pid > 0)
// 		return (cmd->pid = *pid, 0);
// 	if ((sig_init_child() == -1 && utl_exit(EX_FAIL, core))
// 		|| exc_exec_cmd(cmd, core) == -1)
// 		return (-1);
// 	return (0);
// }

static int	exc_init_subsh(t_core *core)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	if (pid > 0)
	{
		// You're a parent now. WHAT?	
		return (0);
	}
	
	// (sig_init_child() == -1 && utl_exit(EX_FAIL, core))
		// || exc_exec_cmd(cmd, core) == -1)
		// return (-1);
	return (0);
}



// 29/12 - That's a draft.
int	exc_exec_ast(int ifd, int ofd, t_astn *root, t_core *core)
{
	if (root->type == AST_AO)
	{
		root->status = exc_exec_ast(ifd, ofd, root->left, core);
		if ((root->op == TOK_AND && root->status == 0)
			|| (root->op == TOK_OR && root->status > 0))
			root->status = exc_exec_ast(ifd, ofd, root->right, core);
	}
	else if (root->type == AST_PI)
	{
		int	*pipe;
		
		if (fmgr_pipe(pipe) == -1)
			return (-1); // 29/12: Unsufficient??
		root->status = exc_exec_ast(pipe[1], ofd, root->left, core);
		if (root->status != -1)
			root->status = exc_exec_ast(ifd, pipe[0], root->right, core);
		if (close(pipe[0]) == -1 || close(pipe[1]) == -1)
			return (-1);
	}
	else if (root->type == AST_SUB)
	{
		// Create sub environment
		// Use ifd and/or ofd to dup2 stdin and stdout
		// 
	}
	else if (root->type == AST_CMD)
	{
		// 
	}
	return (root->status); // ???
}

int	exc_exec_cmds(t_core *core)
{
	// pid_t	pid;
	// int		i;

	// 19/12 - cmd_pmax is obsolete

	// pid = 0;
	// i = -1;
	// while (++i < (core->cmd_pmax + 1))
	// {
	// 	if (core->cmds[i].xready == false
	// 		|| (core->cmd_pmax == 0
	// 			&& exc_if_builtin(core->cmds + i, core)))
	// 		continue ;
	// 	if (exc_init_subsh(i, &pid, core) == -1
	// 		&& exc_wait_cmds(core))
	// 		return (-1);
	// }
	// if (pid > 0 && exc_wait_cmds(core) == -1)
	// 	return (-1);
	(void)core;
	return (0);
}
