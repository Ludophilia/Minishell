/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:06:14 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/12 22:07:22 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// nettoie et quitte le programme avec le code donné
// void	cleanup_and_exit(t_core *core, int code)
// {
// 	utl_cleanup(core->flags, core);
// 	exit(code);
// }

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

// 12/10 
//		* J'ai ajoute cmd->argc pour mieux gerer les cas d'erreurs
//			- Vu que cmd->argc == 0
//   		 	- elle va juste clean and exit dans interface.c, comme tu l'as marque la bas.
// 		* exit status inconsistant. Comment on les gere ici?
int	bi_exit(t_core *core, t_cmd *cmd)
{
	char	**argv;
	long	code;

	argv = cmd->argv;
	ft_printf("exit");
	if (cmd->argc <= 1)
	{
		utl_cleanup(core->flags, core);
		exit(core->exit);
	}
	if (cmd->argc > 2 && ft_eprintf(ERR_XMNY))
	{
		core->exit = 1; // 12/10 - Pourquoi pas de cleanup ici?
		return (1);
	}
	if (!ft_atol_safe(argv[1], &code) && ft_eprintf(ERR_XNUM, argv[1])) 
	{
		utl_cleanup(core->flags, core);
		exit(2);
	}
	utl_cleanup(core->flags, core);
	exit(code % 255);
}

// Original
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
	else if (!ft_atol(argv[1], &code))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		cleanup_and_exit(core, 255);
	}
	if (argv[2]) // 12/10 Unsafe si argv[2] n'existe pas. D'ou l'introduction de argc.
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		g_exit_status = 1;
		return (1);
	}
	cleanup_and_exit(core, (unsigned char)code);
	return (0);
}