/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:55:00 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/09 16:11:58 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_fork(t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror_fail("Minishell: fork()"));
	else if (pid == 0)
	{
		if (close(g_all.end[0]) < 0)
			return (perror_fail("Minishell: close()"));
		if (g_all.nb_cmd != 1 && cmd->position != g_all.nb_cmd)
		{
			if (dup2(g_all.end[1], STDOUT_FILENO) < 0)
				perror_void("Minishell: dup2()");
		}
		if (g_all.nb_cmd != 1 && cmd->position == g_all.nb_cmd)
		{
			if (dup2(g_all.fd_stdout, STDOUT_FILENO) < 0)
				perror_void("Minishell: dup2()");
		}
		if (close(g_all.end[1]) < 0)
			return (perror_fail("Minishell: close()"));
		execute_child(cmd);
	}
	else
	{
		g_all.pid[cmd->position - 1] = pid;
		if (close(g_all.end[1]) < 0)
			return (perror_fail("Minishell: close()"));
		if (g_all.nb_cmd != 1 && cmd->position != g_all.nb_cmd)
		{
			if (dup2(g_all.end[0], STDIN_FILENO) < 0)
				perror_void("Minishell: dup2()");
		}
		if (g_all.nb_cmd != 1 && cmd->position == g_all.nb_cmd)
		{
			if (dup2(g_all.fd_stdin, STDIN_FILENO) < 0)
				perror_void("Minishell: dup2()");
		}
		if (close(g_all.end[0]) < 0)
			return (perror_fail("Minishell: close()"));
		if (cmd->infile_mode != CLASSIC)
			close(cmd->fd_infile);
		if (cmd->outfile_mode != CLASSIC)
			close(cmd->fd_outfile);
	}
	return (SUCCESS);
}

int	execute(t_cmd *cmd_in_global)
{
	t_cmd	*cmd;

	cmd = copy_t_cmd(cmd_in_global);
	if (!cmd)
		return (FAILURE);
	if (is_builtin(cmd) == TRUE && ft_strlen(cmd->wd[0]) == 4 && \
		!ft_strncmp(cmd->wd[0], "exit", 4))
	{
		if (!cmd->wd[1])
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			exit(g_all.status);
		}
		execute_exit(cmd);
	}
	if (ft_fork(cmd) == FAILURE)
	{
		free_t_cmd(cmd);
		return (FAILURE);
	}
	free_t_cmd(cmd);
	return (SUCCESS);
}

void	execute_child(t_cmd *cmd)
{
	if (is_builtin(cmd) == FALSE && find_good_path(cmd) == FAILURE)
		exit(127);
	if (cmd->infile_mode == READ && check_if_openable(cmd) == FAILURE)
		exit(FAILURE);
	if (cmd->outfile_mode != CLASSIC && check_if_writable(cmd) == FAILURE)
		exit(FAILURE);
	if (dup_fd(cmd) == FAILURE)
		exit(FAILURE);
	if (is_builtin(cmd) == TRUE)
		execute_builtin(cmd);
	else
	{
		if (execve(cmd->good_path, cmd->wd, g_all.env) == -1)
		{
			perror("Minishell: execve()");
			exit(127); 
		}
	}
}

int	find_good_path(t_cmd *cmd)
{
	int		i;

	i = 0;
	if (ft_strchr(cmd->wd[0], '/'))
	{
		if (path_full_written(cmd) == FAILURE)
			return (FAILURE);
	}
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