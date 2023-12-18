/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/18 14:34:44 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(char *line)
{
	if (ft_strlen(line) == 4 && ft_strncmp("exit", line, 4) == 0)
	{
		printf("exit\n");
		return (1);
	}
	return (0);
}

int	interf_loop_prompt(t_core *core)
{
	while (1)
	{
		core->line = readline(INTERF_PROMPT);
		if (core->line == NULL)
		{
			printf("[DEBUG] Is that really an error?\n");
			perror("[DEBUG]");
			return (-1);
		}

		if (*core->line != 0)
			add_history(core->line);

		// What should be done here?

		printf("[%i] line = '%s'\n", getpid(), core->line);

		if (builtin_exit(core->line) == 1)
			break ;
		free(core->line);
	}
	return (0);
}

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 2);
		rl_on_new_line();
		rl_replace_line ("", 0);
		rl_redisplay();
	}
}

int	main(void)
{
	t_sigaction	sigaction_s;
	t_core		shellcore;

	sigaction_s.sa_handler = signal_handler;
	if (sigaction(SIGINT, &sigaction_s, NULL) == -1)
		return (2);

	if (interf_loop_prompt(&shellcore) == -1)
		return (1);
	return (0);
}
