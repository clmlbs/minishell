/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 18:58:23 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 19:24:57 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	ft_fork(t_cmd *cmd)
{
	pid_t	pid;

	g_all.where = EXECUTION;
	pid = fork();
	if (pid < 0)
		return (perror_fail("Minishell: fork()"));
	else if (pid == 0)
	{
		g_all.my_pid = 0;
		execute_son(cmd);
	}
	else
	{
		g_all.my_pid = pid;
		g_all.pid[cmd->position - 1] = pid;
		execute_father(cmd);
	}
	return (SUCCESS);
}

void	execute_son(t_cmd *cmd)
{
	if (close(g_all.end[0]) < 0)
		return (perror_void("Minishell: close()"));
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
		return (perror_void("Minishell: close()"));
	execute_child(cmd);
}

void	execute_father(t_cmd *cmd)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
	if (close(g_all.end[1]) < 0)
		return (perror_void("Minishell: close()"));
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
		return (perror_void("Minishell: close()"));
	if (cmd->infile_mode != CLASSIC)
		close(cmd->fd_infile);
	if (cmd->outfile_mode != CLASSIC)
		close(cmd->fd_outfile);
}
