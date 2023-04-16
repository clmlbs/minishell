/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitpid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 16:58:01 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 22:47:41 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	it_s_a_signal(int status)
{
	g_all.status = 128 + WTERMSIG(status);
	if (g_all.status == 131)
		printf("Quit: 3\n");
	else if (WTERMSIG(status) == SIGSEGV)
		ft_putstr_fd("Segmentation fault: 11\n", STDERR_FILENO);
	else if (WTERMSIG(status) == SIGBUS)
		ft_putstr_fd("Bus error: 10\n", STDERR_FILENO);
	else if (WTERMSIG(status) == SIGABRT)
		ft_putstr_fd("Abort trap: 6\n", STDERR_FILENO);
	else if (WTERMSIG(status) == SIGTERM)
		ft_putstr_fd("Terminated: 15\n", STDERR_FILENO);
	else if (WTERMSIG(status) == SIGKILL)
		ft_putstr_fd("Killed: 9\n", STDERR_FILENO);
}

void	ft_waitpid(void)
{
	int	i;
	int	pid;
	int	status;

	i = 0;
	while (i < g_all.nb_cmd)
	{
		pid = waitpid(g_all.pid[i], &status, 0);
		if (pid > 0)
		{
			if (WIFEXITED(status))
				g_all.status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				it_s_a_signal(status);
		}
		if (pid <= 0)
		{
			perror_void("Minishell: waitpid()");
			exit(FAILURE);
		}
		i++;
	}
}
