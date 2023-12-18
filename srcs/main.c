/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:48:39 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/18 17:48:13 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(char *line)
{
	printf("exit\n");
	if (line != NULL)
		free(line);
	return (0);
}

int	interf_loop_prompt(void)
{
	char	*line;

	while (1)
	{
		line = readline(INTERF_PROMPT);
		if (line == NULL && builtin_exit(line) == 0)
			return (0);

		if (*line != 0)
			add_history(line);

		// What should be done here?

		printf("[%i] line = '%s'\n", getpid(), line);

		if ((ft_strlen(line) == 4 && !ft_strncmp("exit", line, 4))
			&& builtin_exit(line) == 0)
			return (0);

		free(line);
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
	t_sigaction	sigaction_s1;
	t_sigaction	sigaction_s2;

	// t_core		shellcore;

	ft_bzero(&sigaction_s1, sizeof(t_sigaction));
	ft_bzero(&sigaction_s2, sizeof(t_sigaction));
	sigaction_s1.sa_handler = signal_handler;
	sigaction_s2.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &sigaction_s1, NULL) == -1
		|| sigaction(SIGQUIT, &sigaction_s2, NULL) == -1)
		return (2);

	if (interf_loop_prompt() == -1)
		return (1);
	return (0);
}
