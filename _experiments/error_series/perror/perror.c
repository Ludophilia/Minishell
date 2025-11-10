/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perror.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 11:56:24 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/02 12:41:55 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
	if (open("evangelion/characters/asuka_langley", O_WRONLY) != -1)
		return (1);
	perror(0);
	perror("This string will be printed followed by ': '"
		", the error message matching errno and a '\\n'");
	return (0);
}
