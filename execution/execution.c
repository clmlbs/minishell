/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:55:00 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 19:21:09 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_line(void)
{
	t_cmd	*buf;

	g_all.pid = (pid_t *)malloc(sizeof(pid_t) * g_all.nb_cmd);
	if (!g_all.pid)
		return (perror_fail("Minishell: malloc()"));
	buf = g_all.cmd;
	while (buf)
	{
		if (g_all.is_first_turn == NO && isatty(STDIN_FILENO) \
			&& update_global() == FAILURE)
			return (FAILURE);
		else
		{
			if (pipe(g_all.end) < 0)
				return (perror_fail("Minishell: pipe()"));
			if (replace_dollar_question_mark(buf->wd) == FAILURE \
				|| execute(buf) == FAILURE)
				return (FAILURE);
			g_all.is_first_turn = NO;
			buf = buf->next;
		}
	}
	return (line_is_executed());
}

int	line_is_executed(void)
{
	ft_waitpid();
	if (g_all.pid)
	{
		free(g_all.pid);
		g_all.pid = NULL;
	}
	g_all.where = PROCESS;
	return (SUCCESS);
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
		execute_echo(cmd, 1);
	else if (ms_strlen(str) == 5 && !ft_strncmp(str, "unset", 5))
		execute_unset(cmd);
	else if (ms_strlen(str) == 6 && !ft_strncmp(str, "export", 6))
		execute_export(cmd, 1, 0);
	else
		exit(FAILURE);
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
		echo_ctl(1);
		if (!cmd->wd || !cmd->wd[0])
			exit(0);
		if (!cmd->wd[0][0])
			cmd_null(cmd);
		check_points(cmd);
		if (execve(cmd->good_path, cmd->wd, g_all.env) == -1)
		{
			perror("Minishell: execve()");
			exit(127);
		}
	}
}
