/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/09 16:35:33 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static int ui_init_core(t_core *core)
{
	core->flags = 0;
	core->cmd_pmax = 0;
	core->cmd_xrdy = 0;
	core->exitv = 0; // ??? Unsure about this one.
	ft_bzero(core->cmds, CMD_MAX * sizeof(t_cmd));
	return (0);
}

static int ui_process_line(char *line, t_core *core)
{
	int psr_exv;

	add_history(line);
	psr_exv = psr_parse_line(line, core);
	if (psr_exv == -2)
		return (-2);
	if (psr_exv == -1 || fmgr_set_reds(core) == -1 || exc_exec_cmds(core) == -1)
	{
		utl_cleanup(core->flags, core);
		return (-1);
	}
	utl_cleanup(core->flags, core);
	return (0);
}

int ui_loop_prompt(t_core *core)
{
	char *line;
	int proc_exv;

	ui_init_core(core); // 7/10 - I may move this in main...
	while (1)
	{
		line = readline(UI_PROMPT);
		if (line == NULL && bi_exit(line) == 0)
			return (0);
		if (*line == 0 && utl_free(line))
			continue;
		proc_exv = ui_process_line(line, core);
		if (proc_exv == -1 && utl_free(line))
			return (-1);
		if (proc_exv == -2 && utl_free(line))
			continue;
		free(line);
	}
	return (0);
}*/
static int	ui_init_core(t_core *core)
{
	core->flags = FLG_ALL;
	core->cmd_pmax = 0;
	core->cmd_xrdy = 0;
	core->exitv = 0;
	ft_bzero(core->cmds, sizeof(core->cmds));
	return (0);
}

static int	ui_process_line(char *line, t_core *core)
{
	int	psr_exv;

	add_history(line);
	psr_exv = psr_parse_line(line, core);
	if (psr_exv == -2)
		return (-2);
	if (psr_exv == -1 || fmgr_set_reds(core) == -1 || exc_exec_cmds(core) == -1)
	{
		utl_cleanup(core->flags, core);
		return (-1);
	}
	utl_cleanup(core->flags, core);
	return (0);
}
// 7/10 - I may move ui_unit_core in main...

int	ui_loop_prompt(t_core *core)
{
	char	*line;
	int		proc_exv;

	ui_init_core(core);
	while (1)
	{
		line = readline(UI_PROMPT);
		if (!line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			cleanup_and_exit(core, g_exit_status);
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		proc_exv = ui_process_line(line, core);
		if (proc_exv == -1)
			return (free(line), -1);
		free(line);
	}
	return (0);
}
