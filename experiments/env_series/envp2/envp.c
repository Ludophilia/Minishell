/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:00:42 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/14 14:06:36 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
	char 	*sep;
	
	while (*envp)
	{
		sep = strchr(*envp, '=');
		*sep = 0;
		printf("%s\n", *envp);
		printf("\t%s\n", sep + 1);
		++envp;
	}
	return (0);
}
