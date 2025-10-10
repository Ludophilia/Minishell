/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:06:14 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/09 15:41:44 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// conversion sécurisée de chaîne en long avec détection des erreurs
int	ft_atol_safe(const char *str, long *res)
{
	long	val;
	long	sign;
	int		digit;

	val = 0;
	sign = 1;
	if (!str || !*str)
		return (0);
	if ((*str == '+' || *str == '-') && *str++ == '-')
		sign = -1;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		digit = *str - '0';
		if (val > (LONG_MAX - digit) / 10)
			return (0);
		val = val * 10 + digit;
		str++;
	}
	*res = (long)(val * sign);
	return (1);
}

// nettoie et quitte le programme avec le code donné
void	cleanup_and_exit(t_core *core, int code)
{
	utl_cleanup(core->flags, core);
	exit(code);
}

int	bi_exit(t_core *core, t_cmd *cmd)
{
	char	**argv;
	long	code;

	argv = cmd->argv;
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!argv[1])
		cleanup_and_exit(core, g_exit_status);
	if (ft_strncmp(argv[1], "-9223372036854775808", 21) == 0)
		code = LONG_MIN;
	else if (!ft_atol_safe(argv[1], &code))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		cleanup_and_exit(core, 255);
	}
	if (argv[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		g_exit_status = 1;
		return (1);
	}
	cleanup_and_exit(core, (unsigned char)code);
	return (0);
}
