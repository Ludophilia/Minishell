/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:18:13 by jgermany          #+#    #+#             */
/*   Updated: 2026/01/04 00:51:59 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	loop_process_line(char *line, t_core *core)
{
	int		psr_exv;

	add_history(line);
	psr_exv = psr_parse_line(line, core);
	if (psr_exv == -2)
		return (-2);
	if (psr_exv == -1 && utl_cleanup(core->flags, 0, core))
		return (-1);

	// 20/12 - Execution. So what's next?
	// You take your AST and execute it, no?

		
	// 10/11, 29/12 - Old execution Part...
	sig_init_exec();
	if (exc_exec_ast(-1, -1, core->ast, core) == -1) // 31/12 - Should be replaced.
	{
		sig_init_prompt();
		utl_cleanup(core->flags, 0, core);
		return (-1);
	}
	sig_init_prompt();
	utl_cleanup(core->flags, FLG_ENV, core);
	return (0);
}

int	loop_prompt(t_core *core)
{
	char	*line;
	int		proc_exv;

	while (1)
	{
		if (sig_init_prompt() == -1)
			return (-1);
		line = readline(UI_PROMPT);
		if (g_sig == SIGINT)
		{
			core->exit = 130;
			g_sig = 0;
		}
		if (line == NULL)
			bi_exit(core, NULL);
		if (*line == 0 && utl_free(line))
			continue ;
		proc_exv = loop_process_line(line, core);
		if (proc_exv == -1 && utl_free(line))
			return (-1);
		if (proc_exv == -2 && utl_free(line))
			continue ;
		free(line);
	}
	return (0);
}
