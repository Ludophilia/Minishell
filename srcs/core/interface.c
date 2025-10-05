/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/06 00:52:11 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	ui_process_line(char *line, t_core *core)
// {
// 	add_history(line);
// 	if (psr_parse_line(line, core) == -1
// 		|| fmgr_set_reds(core) == -1
// 		|| (exc_exec_cmds(core) == -1 && printf("exec problem\n")))
// 	{
// 		utl_cleanup(core->flags, core);
// 		return (free(line), -1);
// 	}
// 	utl_cleanup(core->flags, core);
// 	return (free(line), 0);
// }

int	ui_loop_prompt(t_core *core)
{
	char	*line;
	int		psr_exv;

	ft_bzero(core->cmds, CMD_MAX * sizeof(t_cmd));
	core->flags = 0;
	core->cmd_pmax = 0;
	core->cmd_xrdy = 0;
	while (1)
	{
		line = readline(UI_PROMPT);
		if (line == NULL && bi_exit(line) == 0)
			return (0);
		if (*line == 0 && utl_free(line))
			continue ;
		add_history(line);

		psr_exv = psr_parse_line(line, core);
		if (psr_exv == -2 && utl_free(line))
			continue ;
		if (psr_exv == -1 || fmgr_set_reds(core) == -1
			|| exc_exec_cmds(core) == -1)
		{
			utl_cleanup(core->flags, core);
			return (free(line), -1);
		}

		utl_cleanup(core->flags, core);
		free(line);
	}
	return (0);
}
