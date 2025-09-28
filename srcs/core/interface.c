/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2025/09/28 23:37:31 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	utl_print_cmds(t_core *core)
{
	t_cmd	*cmd;
	int		i;
	int		j;

	i = -1;
	while (++i < (core->cmd_pmax + 1))
	{
		cmd = &core->cmds[i];
		printf("%i\n", i);
		printf("\targs:\n");
		j = -1;
		if (!cmd->argv)
			printf("\t\t(NULL)\n");
		while (cmd->argv && cmd->argv[++j])
			printf("\t\t%s.\n", cmd->argv[j]);
		printf("\tireds:\n");
		if (cmd->ireds[0].type == TOK_EOL)
				printf("\t\t(EOL)\n");
		j = -1;
		while (cmd->ireds[++j].type != TOK_EOL)
			printf("\t\ttype: %i; word: %s\n",
				cmd->ireds[j].type, cmd->ireds[j].word);
		printf("\toreds:\n");
		if (cmd->oreds[0].type == TOK_EOL)
				printf("\t\t(EOL)\n");
		j = -1;
		while (cmd->oreds[++j].type != TOK_EOL)
			printf("\t\ttype: %i; word: %s\n",
				cmd->oreds[j].type, cmd->oreds[j].word);
		printf("\tifds:\n");
		printf("\t\t[%i; %i]\n", cmd->ifds[0], cmd->ifds[1]);
		printf("\tofds:\n");
		printf("\t\t[%i; %i]\n", cmd->ofds[0], cmd->ofds[1]);
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


		fmgr_set_reds(core);


		// 29/09 - We need something to track the opened fds
		utl_print_cmds(core);


		// 29/09 - We count on this for cleaning all fds
		// = That's the test.
		//		= Are all fds closed?
		//		= In every scenario?
		//			= Happy Path
		//			= Errors...
		// 		= valgrind --track-fds=yes to know


		(void)(line && printf("cleanup...\n") && utl_cleanup(core->flags, core)); /// remove void
		utl_print_cmds(core);

		// = Next: Execution.

		// 31/08 - Will certainly move somewhere else as well
		if (ft_strlen(line) == 4 && !ft_strncmp("exit", line, 4))
			return (bi_exit(line), 0);
		free(line);
	}
	return (0);
}
