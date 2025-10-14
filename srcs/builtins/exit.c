/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:06:14 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/14 12:02:51 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// conversion sécurisée de chaîne en long avec détection des erreurs
static int	ft_atol_safe(const char *str, long *res)
{
	int				sign;
	unsigned long	val;
	int				digit;

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
		val = val * 10 + digit;
		str++;
	}
	*res = (long)(val * sign);
	return (1);
}

int	bi_exit(t_core *core, t_cmd *cmd)
{
	long	code;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (cmd->argc < 2)
	{
		utl_cleanup(core->flags | FLG_ENV, core);
		exit(core->exit);
	}
	if (cmd->argc > 2)
	{
		ft_eprintf(ERR_XMNY);
		return (core->exit = 1);
	}
	if (!ft_atol_safe(cmd->argv[1], &code))
	{
		ft_eprintf(ERR_XNUM, cmd->argv[1]);
		code = 2;
	}
	utl_cleanup(core->flags | FLG_ENV, core);
	exit(core->exit = code % 255);
}
