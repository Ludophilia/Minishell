/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2025/09/26 19:46:52 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	utl_print_cmds(t_core *core)
{
	t_cmd	*cmd;
	int		i;
	int		j;

	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = &core->cmds[i];
		printf("%i\n", i);
		printf("\t=== args ===\n");
		j = -1;
		if (!cmd->argv)
			printf("\t(NULL)\n");
		while (cmd->argv && cmd->argv[++j])
			printf("\t%s.\n", cmd->argv[j]);
		printf("\t=== ireds ===\n");
		j = -1;
		while (cmd->ireds[++j].type != TOK_EOL)
			printf("\ttype: %i; word: %s\n",
				cmd->ireds[j].type, cmd->ireds[j].word);
		printf("\t=== oreds ===\n");
		j = -1;
		while (cmd->oreds[++j].type != TOK_EOL)
			printf("\ttype: %i; word: %s\n",
				cmd->oreds[j].type, cmd->oreds[j].word);
		printf("\n");
	}
	return (0);
}

int	ui_loop_prompt(t_core *core)
{
	char	*line;

	ft_bzero(core->cmds, CMD_MAX * sizeof(t_cmd));
	core->flags = 0;
	core->cmd_pmax = 0;
	while (1)
	{
		line = readline(UI_PROMPT);
		if (line == NULL && bi_exit(line) == 0)
		{
			utl_cleanup(core->flags, core);
			return (0);
		}

		if (*line != 0)
			add_history(line);
		

		// 5/09 - Not at the right level anyway...
		if (*line != 0 && psr_parse_line(line, core) == -1)
		{
			free(line);
			continue ; // 7/09 - Find a better way.
		}

		// 22/09, 26/09 - What should be done??

		
		utl_print_cmds(core);
		// Past that point. the structures are filled up.
		
		// Open redirections / pipes / and set the right things into the right structure.
		// 26
		// fmgr_set_reds(core); // == -1...

	

		
		(void)(line && utl_cleanup(core->flags, core)); /// ???

		// 31/08 - Will certainly move somewhere else as well
		if (ft_strlen(line) == 4 && !ft_strncmp("exit", line, 4))
			return (bi_exit(line), 0);
		free(line);
	}
	return (0);
}
