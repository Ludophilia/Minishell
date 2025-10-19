/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:06:14 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/19 03:09:47 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_atol_safe(const char *str, long *res)
{
	int				sign;
	unsigned long	val;

	if (!str || !*str)
		return (0);
	sign = 1;
	val = 0;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		val = val * 10 + (*str - '0');
		if (val > LONG_MAX
			&& !(sign == -1 && val == (unsigned long)LONG_MAX + 1))
			return (0);
		str++;
	}
	*res = (long)(val * sign);
	return (1);
}

int	bi_exit(t_core *core, t_cmd *cmd)
{
	long	code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (cmd->argc < 2)
	{
		utl_cleanup(core->flags | FLG_ENV, core);
		exit(core->exit);
	}
	if (cmd->argc > 2)
	{
		ft_eprintf(ERR_BMNY, "exit");
		return (core->exit = 1);
	}
	if (!ft_atol_safe(cmd->argv[1], &code))
	{
		ft_eprintf(ERR_BINV, "exit", cmd->argv[1]);
		utl_exit(2, core);
	}
	utl_cleanup(core->flags | FLG_ENV, core);
	exit(core->exit = (unsigned char)code);
}
