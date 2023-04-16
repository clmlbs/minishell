/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 18:55:35 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 19:12:32 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_if_openable(t_cmd *cmd)
{
	DIR	*dir;

	dir = opendir(cmd->infile_name);
	if (dir)
	{
		closedir(dir);
		write_error("Minishell: ", cmd->infile_name, " Is a directory\n");
		return (FAILURE);
	}
	if (access(cmd->infile_name, F_OK) == -1)
		return (perror_fail("Minishell: access"));
	cmd->fd_infile = open(cmd->infile_name, O_RDONLY);
	if (cmd->fd_infile == -1)
		return (perror_fail("Minishell"));
	return (SUCCESS);
}

int	check_if_writable(t_cmd *cmd)
{
	DIR	*dir;

	dir = opendir(cmd->outfile_name);
	if (dir)
	{
		closedir(dir);
		write_error("Minishell: ", cmd->outfile_name, " Is a directory\n");
		return (FAILURE);
	}
	if (cmd->outfile_mode == REPLACE)
		cmd->fd_outfile = open(cmd->outfile_name, \
			O_CREAT | O_RDWR | O_TRUNC, 0666);
	else if (cmd->outfile_mode == APPEND)
		cmd->fd_outfile = open(cmd->outfile_name, \
			O_CREAT | O_RDWR | O_APPEND, 0666);
	if (cmd->fd_outfile == -1)
		return (perror_fail("Minishell"));
	return (SUCCESS);
}

void	check_points(t_cmd *cmd)
{
	if (!cmd->wd[1])
	{
		if (!ft_strcmp(cmd->wd[0], ".") && ms_strlen(cmd->wd[0]) == 1)
		{
			ft_putstr_fd("Minishell: .: filename argument required\n", 2);
			ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
			exit(2);
		}
		if (!ft_strcmp(cmd->wd[0], "..") && ms_strlen(cmd->wd[0]) == 2)
		{
			ft_putstr_fd("Minishell: ..: command not found\n", 2);
			exit(127);
		}
	}
}

int	find_good_path(t_cmd *cmd)
{
	int		i;

	i = 0;
	if (ft_strchr(cmd->wd[0], '/'))
		return (path_contain_slash(cmd));
	while (g_all.all_path && g_all.all_path[i])
	{
		cmd->good_path = ms_strjoin(g_all.all_path[i], cmd->wd[0]);
		if (!cmd->good_path)
			return (FAILURE);
		if (access(cmd->good_path, X_OK) == 0)
			return (SUCCESS);
		free(cmd->good_path);
		i++;
	}
	write_error("Minishell: ", cmd->wd[0], ": command not found\n");
	return (FAILURE);
}

int	path_contain_slash(t_cmd *cmd)
{
	if (access(cmd->wd[0], X_OK) == 0)
	{
		cmd->good_path = ft_strdup(cmd->wd[0]);
		if (!cmd->good_path)
			return (FAILURE);
		if (create_executable_name(cmd) == FAILURE)
			return (FAILURE);
		return (SUCCESS);
	}
	else
	{
		if (errno == EACCES)
		{
			write_error("Minishell: ", cmd->wd[0], ": Permission denied\n");
			exit(126);
		}
		write_error("Minishell: ", cmd->wd[0], ": No such file or directory\n");
		return (FAILURE);
	}
}
