/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 22:43:08 by jegerman          #+#    #+#             */
/*   Updated: 2025/12/18 19:18:38 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	psr_cleanup_cmd(t_cmd *cmd)
{
	cmd->xready = 0;
	cmd->pid = 0;
	if (cmd->argv == NULL)
		return (1);
	utl_free_strs(0, cmd->argv);
	cmd->argv = NULL;
	cmd->argc = 0;
	return (1);
}

static int	psr_cleanup_red(t_cmd *cmd)
{
	t_red	*red;

	red = cmd->reds;
	while (red->type != TOK_EOL)
	{
		red->type = TOK_EOL;
		if (red->word)
			free(red->word);
		red->word = NULL;
		red++;
	}
	cmd->ifd = (fmgr_close(&cmd->ifd), 0);
	cmd->ofd = (fmgr_close(&cmd->ofd), 1);
	return (1);
}

int	psr_cleanup_ast(t_astn *root)
{
	if (root->left)
		psr_cleanup_ast(root->left);
	if (root->right)
		psr_cleanup_ast(root->right);
	if (root->content)
	{
		psr_cleanup_cmd(root->content);
		psr_cleanup_red(root->content);		
	}
	free(root);
	return (0);
}
