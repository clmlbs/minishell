/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 15:12:12 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/12 09:09:05 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signal_sigint(void)
{
	if (g_all.where == DAD)
	{
		g_all.status = 1;
		//printf("\e[2K");
		//rl_on_new_line();
		//rl_redisplay();
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (g_all.where == SON)
	{
		g_all.status = 130;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	else if (g_all.where == HERE_DOC)
	{
		(void)signal;
		if (g_all.my_pid == 0)
			exit(130);
		else
		{
			rl_redisplay();
			printf("\n");
		}
		g_all.status = 130;
	}
}

void	signal_sigquit(void)
{
	if (g_all.where == DAD || g_all.where == HERE_DOC)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	if (g_all.where == SON)
	{
		printf("Quit: 3\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		g_all.status = 131;
	}
}

void	signal_handle(int signal)
{
	if (signal == SIGINT)
		signal_sigint();
	if (signal == SIGQUIT)
		signal_sigquit();
}
