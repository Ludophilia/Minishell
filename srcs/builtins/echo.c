/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:05:51 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/17 20:12:30 by jegerman         ###   ########.fr       */
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
static int	echo_print_args(char **args, int i, int fd)
{
	while (args[i])
	{
		if (ft_dprintf(fd, "%s", args[i]) == -1)
			return (-1);
		if (args[i + 1])
		{
			if (ft_dprintf(fd, " ") == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	bi_echo(t_cmd *cmd, int fd)
{
	int		i;
	int		newline;
	char	**args;

	if (!cmd || !cmd->argv)
		return (1);
	args = cmd->argv;
	i = 1;
	newline = 1;
	while (args[i] && is_valid_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	if (echo_print_args(args, i, fd) == -1)
		return (1);
	if (newline)
	{
		if (ft_dprintf(fd, "\n") == -1)
			return (1);
	}
	return (0);
}
