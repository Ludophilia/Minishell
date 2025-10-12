/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/12 21:42:43 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ui_process_line(char *line, t_core *core)
{
	int		psr_exv;

	add_history(line);
	psr_exv = psr_parse_line(line, core);
	if (psr_exv == -2)
		return (-2);
	if (psr_exv == -1
		|| fmgr_set_reds(core) == -1
		|| exc_exec_cmds(core) == -1)
	{
		utl_cleanup(core->flags, core);
		return (-1);
	}
	utl_cleanup(core->flags, core);
	return (0);
}

int	ui_loop_prompt(t_core *core)
{
	char	*line;
	int		proc_exv;

	while (1)
	{
		line = readline(UI_PROMPT);
		if (line == NULL)
			bi_exit(core, *core->cmds);
		if (*line == 0 && utl_free(line))
			continue ;
		proc_exv = ui_process_line(line, core);
		if (proc_exv == -1 && utl_free(line))
			return (-1);
		if (proc_exv == -2 && utl_free(line))
			continue ;
		free(line);
	}
	return (0);
}
