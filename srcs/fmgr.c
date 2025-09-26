/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmgr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:04:49 by jegerman          #+#    #+#             */
/*   Updated: 2025/09/26 20:10:08 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define ERR_GNR "minishell: %s\n"
# define ERR_PTH "minishell: %s: %s\n"
# define ERR_CMD "minishell: %s: command not found\n"
# define ENV_PATH_DFLT "/bin:/usr/bin"
# define FL_PRMS 00664

// ##########################################################################


// Logic to check from the envp if a file can be executed
// Execve

int	pgmb_free_strs(int from_id, char **strs)
{
	int	i;

	if (strs == NULL)
		return (-1);
	i = 0;
	while (strs[from_id + i])
		free(strs[from_id + i++]);
	free(strs);
	return (1);
}

static char	**ptbb_load_env_paths(char **envp)
{
	char	**paths;
	int		is_dflt;
	int		i;

	is_dflt = 1;
	i = -1;
	while (envp[++i])
		if (ft_strnstr(envp[i], "PATH", 4) && is_dflt--)
			break ;
	if (is_dflt)
		paths = ft_split(ENV_PATH_DFLT, ':');
	else
		paths = ft_split(envp[i] + 5, ':');
	return (paths);
}

// Builds the full path for the program, if needed.
static char	*ptbb_build_path(char **strs)
{
	char	*path;
	size_t	path_len;
	int		i;

	i = 0;
	path_len = 0;
	while (strs[i])
		path_len += ft_strlen(strs[i++]);
	path = ft_calloc(path_len + 1, sizeof(char));
	if (path == NULL)
		return (NULL);
	i = -1;
	while (strs[++i])
	{
		path_len = ft_strlen(path) + ft_strlen(strs[i]);
		if (ft_strlcat(path, strs[i], path_len + 1) != path_len)
		{
			free(path);
			return (NULL);
		}
	}
	return (path);
}

// returns 0. When a file is found and executable. 
int	ptbb_check_path(char **cmd_args, char **envp)
{
	char	**paths;
	char	*new_path;
	int		i;

	if (ft_strchr(*cmd_args, '/'))
		return (fmgr_access(*cmd_args, X_OK));
	paths = ptbb_load_env_paths(envp);
	if (paths == NULL)
		return (-1);
	i = -1;
	while (paths[++i])
	{
		new_path = ptbb_build_path((char *[]){paths[i], "/", cmd_args[0], 0});
		if (new_path == NULL && pgmb_free_strs(0, paths))
			return (-1);
		if (access(new_path, X_OK) == 0 && pgmb_free_strs(0, paths))
		{
			free(*cmd_args);
			*cmd_args = new_path;
			return (0);
		}
		free(new_path);
	}
	(ft_eprintf(ERR_CMD, *cmd_args), pgmb_free_strs(0, paths));
	return (-1);
}

// ########################################################################

// All of these are utils.

int	fmgr_access(char *path, int type)
{
	if (access(path, type) == -1)
	{
		ft_eprintf(ERR_PTH, path, strerror(errno));
		return (-1);
	}
	return (0);
}

int	fmgr_open(char *path, int openflags, mode_t openmode)
{
	int	fd;

	if (path == NULL)
		return (-1);
	fd = open(path, openflags, openmode);
	if (fd == -1 && ft_eprintf(ERR_PTH, path, strerror(errno)))
		return (-1);
	return (fd);
}

int	fmgr_pipe(int fds[2])
{
	if (pipe(fds) == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	return (0);
}

int	fmgr_close(int fd)
{
	// if (prg_fds[end] <= 2) // ???
	// 	return (0);
	if (close(fd) == -1 && ft_eprintf(ERR_GNR, strerror(errno)))
		return (-1);
	// prg_fds[end] = -1;
	return (0);
}

// int	fmgr_closeall(int from_id, int reverse, t_pgb *prgs)
// {
// 	t_pgb	prg;

// 	if (access(HRDC_NAME, F_OK) == 0)
// 		unlink(HRDC_NAME);
// 	while ((reverse && from_id >= 0) || (!reverse && prgs[from_id].cmd))
// 	{
// 		prg = prgs[from_id];
// 		if ((from_id == 0 && fmgr_close(prg.in, 0) == -1)
// 			|| fmgr_close(prg.out, 0) == -1
// 			|| fmgr_close(prg.out, 1) == -1)
// 			return (-1);
// 		if (reverse)
// 			from_id--;
// 		else
// 			++from_id;
// 	}
// 	return (1);
// }

// #################################################

// 26/06

// Open every type of redirections.
//	= Open the pipes

//	= Open the '<' 
//	= Open the '<<'

//	= Open the '>'
//	= Open the '>>'

// If a red has precedence, you close either the read end (0), 
// or the write end (1) or either side...

// SO start with pipes.

int fmgr_set_pipe(int pos, int pmax, t_cmd *cmd)
{
	t_cmd	*cmd;

	if (pmax == 0)
		return (0);
	if (pos != pmax && fmgr_pipe(cmd->ofds) == -1)
	{
		// close_all fds that has been opened so far ??
		return (-1);
	}
	if (pos != 0)
	{
		cmd->ifds[0] = (cmd - 1)->ofds[0];
		cmd->ifds[1] = (cmd - 1)->ofds[1];
	}
	return (0);
}

int	fmgr_set_ired(int *ifds, t_red *red)
{
	int	fd;

	fd = fmgr_open(red->word, O_RDONLY, FL_PRMS);
	if (fd == -1)
		return (-1);
	if (ifds[0] != 0) // guaranteed?
	{
		fmgr_close(ifds[0]);
	}

	return ();
}

// int	fmgr_set_hdocs()
// {
	
// }

int	fmgr_set_ireds(int pos, t_cmd *cmd)
{
	t_red	*ireds;
	int		j;

	j = 0;
	ireds = cmd->ireds;
	while (ireds[j].type != TOK_EOL)
	{
		// open file, get the fd, and modify ifds
		
		if (ireds[j].type == TOK_IRED)
			fmgr_set_ired(cmd->ifds, ireds + j);// 

		else if (ireds[j].type == TOK_IRED_HD)
			// 
		j++;
	}
	return (0);
}

int	fmgr_set_reds(t_core *core)
{
	int		i;
	t_cmd	*cmd;

	i = 0;
	while (i < (core->cmd_pmax + 1))
	{
		cmd = core->cmds + i;
		if (fmgr_set_pipe(i, core->cmd_pmax, cmd) == -1
			|| fmgr_set_ired(cmd, i) == -1)
			return (-1);
		


		// Treat the input redirection next
		// if == -1
		

		// Treat the output redirection afterwards

		
		
		i++; // Does it really increnent?
	}
	return (0);
}
