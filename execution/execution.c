/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:55:00 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/04 09:22:19 by cleblais         ###   ########.fr       */
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

int	execute(t_cmd *cmd_in_global)
{
	t_cmd	*cmd;

	cmd = copy_t_cmd(cmd_in_global);
	if (!cmd)
		return (FAILURE);
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
		exit(1); // trouver le bon code
	if (cmd->infile_mode == READ && check_if_openable(cmd) == FAILURE)
		exit(1); // trouver le bon code 
	if (cmd->outfile_mode != CLASSIC && check_if_writable(cmd) == FAILURE)
		exit(1); // trouver le bon code 
	if (dup_fd(cmd) == FAILURE)
		exit(1);//trouver le bon code
	if (is_builtin(cmd) == TRUE)
		execute_builtin(cmd);
	else
	{
		if (execve(cmd->good_path, cmd->wd, g_all.env) == -1)
		{	
			perror_void("Minishell: execve()");
			exit(1);// trouver le bon code 
		}
	}
}

void	execute_builtin(t_cmd *cmd)
{
	// if (ft_strlen(cmd->wd[0]) == 2 && !ft_strncmp(cmd->wd[0], "cd", 2))
	// 	execute_cd(cmd);
	/*else*/if (ft_strlen(cmd->wd[0]) == 3 && !ft_strncmp(cmd->wd[0], "pwd", 3))
		execute_pwd(cmd);
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
		exit(1);// je vois pas comment on peut etre else mais au cas ou pour pas segfault
}