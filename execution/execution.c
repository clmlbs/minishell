/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:55:00 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/04 08:56:04 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_fork(t_cmd *cmd)
{
	pid_t	pid;
	int		end[2];

	if (pipe(end) < 0)
		return (perror_failure("Minishell: pipe()"));
	pid = fork();
	if (pid < 0)
		return (perror_failure("Minishell: fork()"));
	else if (pid == 0)
	{
		if (close(end[0] < 0))
			return (perror_failure("Minishell: close()"));
		execute_child(cmd);
	}
	else
	{
		if (close(end[1]) < 0)
			return (perror_failure("Minishell: close()"));
		if (dup2(STDIN_FILENO, end[0]) < 0)
			perror_void("Minishell: dup2()");
		if (close(end[0]) < 0)
			return (perror_failure("Minishell: close()"));
	}
	return (SUCCESS);
}

int	is_builtin(t_cmd *cmd)
{
	// if (ft_strlen(cmd->wd[0]) == 2 && !ft_strncmp(cmd->wd[0], "cd", 2))
	// 	execute_cd(cmd);
	/*else*/if (ft_strlen(cmd->wd[0]) == 3 && !ft_strncmp(cmd->wd[0], "pwd", 3))
		execute_pwd();
	else if (ft_strlen(cmd->wd[0]) == 3 && !ft_strncmp(cmd->wd[0], "env", 3))
		execute_env(cmd);
	// else if (ft_strlen(cmd->wd[0]) == 4 && !ft_strncmp(cmd->wd[0], "echo", 4))
	// 	execute_echo(cmd);
	// else if (ft_strlen(cmd->wd[0]) == 4 && !ft_strncmp(cmd->wd[0], "exit", 4))
	// 	execute_exit();
	// else if (ft_strlen(cmd->wd[0]) == 5 && !ft_strncmp(cmd->wd[0], "unset", 5))
	// 	execute_unset(cmd);
	// else if (ft_strlen(cmd->wd[0]) == 6 && !ft_strncmp(cmd->wd[0], "export", 6))
	// 	execute_export(cmd);
	else
		return (FALSE);
	return (TRUE);
}

int	execute(t_cmd *cmd_in_global)
{
	t_cmd	*cmd;

	cmd = copy_t_cmd(cmd_in_global);
	if (!cmd)
		return (FAILURE);
	if (is_builtin(cmd) == TRUE)
		g_all.nb_cmd--; // pour qu'il n'y ait pas de pb de waitpid ? 
	else
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