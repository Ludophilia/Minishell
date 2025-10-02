/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:05:43 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/01 23:58:14 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_exit(char *line)
{
	printf("exit\n");
	if (line)
		free(line);
	// exit(EXIT_SUCCESS);
	// utl_cleanup(t_cflg flags, t_core *core);
	return (0);
}
