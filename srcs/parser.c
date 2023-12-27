/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 14:36:28 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/27 14:46:54 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	psr_is_type(enum e_typ type, char *line)
{
	int	i;

	if (line == NULL)
		return (-1);
	i = 0;
	while (type == TY_SPEC && SPECIAL_CHARS[i])
	{
		if (SPECIAL_CHARS[i++] == *line)
			return (1);
	}
	while (type == TY_CMDEND && psr_is_type(TY_SPAC, line + i))
	{
		if (line[i + 1] == '\0' || psr_is_type(TY_SPEC, line + i + 1) == 1)
			return (1);
		i++;
	}
	if (type == TY_SPAC && (*line == ' ' || (*line >= '\t' && *line <= '\r')))
		return (1);
	else if (type == TY_RDIN && (*line == '<' && *(line + 1) != '<'))
		return (1);
	return (0);
}

int	psr_extract_cmd(int *i, char *line)
{
	char	*cmd;
	int		j;

	if (i == NULL)
		return (-1);
	j = 0;
	while (line[j] && psr_is_type(TY_SPEC, line + j) == 0)
	{
		if (psr_is_type(TY_CMDEND, line + j) == 1)
			break ;
		j++;
	}
	cmd = ft_substr(line, 0, j - 0);
	if (cmd == NULL)
		return (-1);
	*i += j;
	printf("cmd = '%s'\n", cmd); // REMOVE
	free(cmd);
	return (0);
}

int	psr_extract_path(char *name, int *i, char *line)
{
	char	*path;
	int		j;
	int		k;

	if (i == NULL)
		return (-1);
	j = 0;
	while (line[j] && psr_is_type(TY_SPAC, line + j) == 1)
		j++;
	k = j;
	while (line[k] && psr_is_type(TY_SPAC, line + k) == 0)
		k++;
	path = ft_substr(line, j, (k - j));
	if (path == NULL)
		return (-1);
	*i += k + 1;
	printf("%s = '%s'\n", name, path); // REMOVE
	free(path);
	return (0);
}
