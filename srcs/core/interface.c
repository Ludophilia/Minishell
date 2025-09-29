/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2025/09/29 20:32:39 by jegerman         ###   ########.fr       */
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
		printf("\txready:\n");
		printf("\t\t%i\n", cmd->xready);
		printf("\targs:\n");
		j = -1;
		if (!cmd->argv)
			printf("\t\t(NULL)\n");
		while (cmd->argv && cmd->argv[++j])
			printf("\t\t%s.\n", cmd->argv[j]);
		printf("\treds:\n");
		if (cmd->reds[0].type == TOK_EOL)
				printf("\t\t(EOL)\n");
		j = -1;
		while (cmd->reds[++j].type != TOK_EOL)
			printf("\t\ttype: %i; word: %s\n",
				cmd->reds[j].type, cmd->reds[j].word);
		printf("\tifd:\n");
		printf("\t\t[%i]\n", cmd->ifd);
		printf("\tofd:\n");
		printf("\t\t[%i]\n", cmd->ofd);
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
		if (line && psr_parse_line(line, core) == -1) //*line != 0 
		{
			free(line);
			utl_cleanup(core->flags, core);
			continue ; // 7/09 - Find a better way.
		}

		fmgr_set_reds(core);

		utl_print_cmds(core);

		(void)(line && printf("cleanup...\n") && utl_cleanup(core->flags, core)); /// remove void
		// = Next: Execution.

		// utl_print_cmds(core);
		
		// 31/08 - Will certainly move somewhere else as well
		if (ft_strlen(line) == 4 && !ft_strncmp("exit", line, 4))
			return (bi_exit(line), 0);
		free(line);
	}
	return (0);
}
