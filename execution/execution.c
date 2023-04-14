/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:55:00 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/14 17:20:36 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_fork(t_cmd *cmd)
{
	pid_t	pid;

	g_all.where = EXECUTION;
	pid = fork();
	if (pid < 0)
		return (perror_fail("Minishell: fork()"));
	else if (pid == 0)
	{
		//signal(SIGQUIT, signal_handle); Normalement osef puisque cest le fils 
		g_all.my_pid = 0;
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
		// if (pid != getpid())
		// {
			struct sigaction sa;
            sa.sa_handler = SIG_IGN;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sigaction(SIGQUIT, &sa, NULL);
		// }
		g_all.my_pid = pid;
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
	if (is_builtin(cmd) == TRUE && ms_strlen(cmd->wd[0]) == 4 && \
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
	{
		printf("builtin\n");//******
		execute_builtin(cmd);
	}
	else
	{
		echo_ctl(1);
		if (!cmd->wd || !cmd->wd[0])
			exit(0);
		check_points(cmd);
		if (execve(cmd->good_path, cmd->wd, g_all.env) == -1)
		{
			perror("Minishell: execve()");
			exit(127); 
		}
	}
}

void	execute_builtin(t_cmd *cmd)
{
	char	*str;
	
	str = put_to_lower(cmd->wd[0]);
	if (!str)
		exit(FAILURE);	
	if (ms_strlen(str) == 2 && !ft_strncmp(str, "cd", 2))
		execute_cd(cmd);
	else if (ms_strlen(str) == 3 && !ft_strncmp(str, "pwd", 3))
		execute_pwd(cmd);
	else if (ms_strlen(str) == 3 && !ft_strncmp(str, "env", 3))
		execute_env(cmd);
	else if (ms_strlen(str) == 4 && !ft_strncmp(str, "echo", 4))
		execute_echo(cmd);
	else if (ms_strlen(str) == 5 && !ft_strncmp(str, "unset", 5))
		execute_unset(cmd);
	else if (ms_strlen(str) == 6 && !ft_strncmp(str, "export", 6))
		execute_export(cmd);
	else
		exit(FAILURE);
}