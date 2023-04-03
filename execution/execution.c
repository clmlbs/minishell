/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:55:00 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/03 14:12:09 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_if_openable(t_cmd *cmd)
{
	DIR	*dir;
	int	fd;

	dir = opendir(cmd->infile_name);
	if (dir)
	{
		closedir(dir);
		write_error("Minishell: ", cmd->infile_name, " Is a directory\n");
		return (FAILURE);
	}
	if (access(cmd->infile_name, F_OK) == -1)
		return (ft_perror("Minishell"));
	fd = open(cmd->infile_name, O_RDONLY);
	if (fd == -1)
		return (ft_perror("Minishell"));
	close(fd);
	return (SUCCESS);
}

int	ft_fork(t_cmd *cmd)
{
	pid_t	pid;
	int		end[2];

	if (pipe(end) < 0)
		return (ft_perror("Minishell: pipe()"));
	pid = fork();
	if (pid < 0)
		return (ft_perror("Minishell: fork()"));
	else if (pid == 0)
	{
		if (close(end[0] < 0))
			return (ft_perror("Minishell: close()"));
		if (execute_child(cmd) == FAILURE)
			return (FAILURE);
	}
	else
	{
		if (close(end[1]) < 0)
			return (ft_perror("Minishell: close()"));
		if (dup2(end[0], STDIN_FILENO) < 0)
			error_dup(end[0]);
		if (close(end[0]) < 0)
			return (ft_perror("Minishell: close()"));
	}
	return (SUCCESS);
}

int	execute(t_cmd *cmd_in_global)
{
	t_cmd	*cmd;

	if (cmd_in_global->infile_name && \
		check_if_openable(cmd_in_global) == FAILURE)
		return (FAILURE);
	cmd = copy_t_cmd(cmd_in_global);
	if (!cmd)
		return (FAILURE);
	// if (is_builtin(cmd) == TRUE)
	// 	builtin(cmd);
	// else
	{	
		if (ft_fork(cmd) == FAILURE)
		{
			free_t_cmd(cmd);
			return (FAILURE);
		}
	}
	free_t_cmd(cmd);
	return (SUCCESS);
}