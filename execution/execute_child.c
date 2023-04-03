/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 12:01:56 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/03 18:12:49 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_good_path(t_cmd *cmd)
{
	int		i;

	i = 0;
	while (g_all.all_path[i])
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
		return (perror_failure("Minishell: access"));
	cmd->fd_infile = open(cmd->infile_name, O_RDONLY);
	if (cmd->fd_infile == -1)
		return (perror_failure("Minishell"));
	//close(fd); normalement pas besoin de le close puisqu'on va l'exec 
	return (SUCCESS);
}

int check_if_writable(t_cmd *cmd)
{
	DIR	*dir;

	dir = opendir(cmd->outfile_name);
	if (dir)
	{
		closedir(dir);
		write_error("Minishell: ", cmd->outfile_name, " Is a directory\n");
		return (FAILURE);
	}
	// if (access(cmd->outfile_name, W_OK) == -1)
	// 	return (perror_failure("Minishell: access"));
	if (cmd->outfile_mode == REPLACE)
		cmd->fd_outfile = open(cmd->outfile_name, \
			O_CREAT | O_RDWR | O_TRUNC, 0666);
	else if (cmd->outfile_mode == APPEND)
		cmd->fd_outfile = open(cmd->outfile_name, \
			O_CREAT | O_RDWR | O_APPEND, 0666);
	if (cmd->fd_infile == -1)
		return (perror_failure("Minishell"));
	return (SUCCESS);
}

int	dup_fd(t_cmd *cmd)
{
	if (cmd->infile_mode == READ || cmd->infile_mode == HERE_DOC)
	{
		if (dup2(cmd->fd_infile, STDIN_FILENO) == -1)
			return (FAILURE);
	}
	if (cmd->outfile_mode != CLASSIC)
	{
		if (dup2(cmd->fd_outfile, STDOUT_FILENO) == -1)
			return (FAILURE);
	}
	return (SUCCESS);
}

void	execute_child(t_cmd *cmd)
{
	if (find_good_path(cmd) == FAILURE)
		exit(1); // trouver le bon code
	if (cmd->infile_mode == READ && check_if_openable(cmd) == FAILURE)
		exit(1); // trouver le bon code 
	if (cmd->outfile_mode != CLASSIC && check_if_writable(cmd) == FAILURE)
		exit(1); // trouver le bon code 
	if (dup_fd(cmd) == FAILURE)
		exit(1);//trouver le bon code
	if (execve(cmd->good_path, cmd->wd, g_all.env) == -1)
	{	
		perror_void("Minishell: execve()");
		exit(1);// trouver le bon code 
	}
}