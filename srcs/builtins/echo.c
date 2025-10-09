/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:05:51 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/09 15:28:12 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// pour les differentes formes possibles de -n
static int	is_valid_n_option(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

// affiche les arguments de echo
static void	echo_print_args(char **args, int i, int fd)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], fd);
		if (args[i + 1])
			ft_putstr_fd(" ", fd);
		i++;
	}
}

int	bi_echo(t_cmd *cmd, int fd)
{
	int		i;
	int		newline;
	char	**args;

	if (!cmd || !cmd->argv)
		return (-1);
	args = cmd->argv;
	i = 1;
	newline = 1;
	while (args[i] && is_valid_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	echo_print_args(args, i, fd);
	if (newline)
		ft_putstr_fd("\n", fd);
	return (0);
}
