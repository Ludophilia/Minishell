/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 13:56:38 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/26 14:04:28 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libs/libft/includes/libft.h"
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	*str;

	str = ft_strtrim("      nyan      ", " ");
	printf("str = '%s'\n", str);
	free(str);
	
	str = ft_strtrim("(<nyan> | <wan && kon>) || \"moou\"",
		"<>()|\"'&");
	printf("str = '%s'\n", str);
	free(str);
	return (0);
}