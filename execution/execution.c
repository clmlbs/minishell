/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:55:00 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/04 09:11:37 by cleblais         ###   ########.fr       */
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