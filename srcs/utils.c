/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:18:23 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/19 00:46:05 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	utils_cleanup(t_cflg flags, t_core *core)
{
	if (flags & FLG_CMDS)
		psr_cleanup_cmds(core);
	return (1);
}
