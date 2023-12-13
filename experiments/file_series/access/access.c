/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 15:27:08 by jgermany          #+#    #+#             */
/*   Updated: 2023/11/27 14:44:51 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

#define CLI_FNE "'%s' doesn't EVEN exist 😂\n"

#define CLI_FR "The process has read access on '%s', same as the user.\n"
#define CLI_FW "The process has write access on '%s', same as the user.\n"
#define CLI_FX "The process has execution rights on '%s', same as the user.\n"
#define CLI_FRWX "The process has all rights on '%s', same as the user.\n"
#define CLI_FNRWX "The process does not have all rights on '%s', same as the user.\n"

#define MAKEFILE "../../../Makefile"
#define FILE "file"
#define ACCESS_C "access.c"
#define FORBIDDEN "secret_file"

void	main_check_rights(char *filename)
{
	if (access(filename, F_OK) == -1 && dprintf(2, CLI_FNE, filename))
		return ;
	if (access(filename, R_OK) == 0)
		dprintf(1, CLI_FR, filename);
	if (access(filename, W_OK) == 0)
		dprintf(1, CLI_FW, filename);
	if (access(filename, X_OK) == 0)
		dprintf(1, CLI_FX, filename);
	if (access(filename, R_OK | W_OK | X_OK) == 0)
		dprintf(1, CLI_FRWX, filename);
	else
		dprintf(2, CLI_FNRWX, filename);
}

int		main(void)
{
	main_check_rights(FILE);
	printf("\n");
	main_check_rights(MAKEFILE);
	printf("\n");
	main_check_rights(ACCESS_C);
	printf("\n");
	main_check_rights(FORBIDDEN);
	return (0);
}