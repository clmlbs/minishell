/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 15:12:12 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/10 16:38:39 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signal_main(int signal)
{
	if (signal == SIGINT && g_all.where == DAD)
	{
		g_all.status = 1;
		printf("\e[2K");
        rl_on_new_line();
        rl_redisplay();
        printf("\n");
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
	}
	else if (signal == SIGINT && g_all.where == SON)
	{
		g_all.status = 130;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

// void	signal_handle(int signal)
// {
// 	if (g_all.my_pid == 0)// && !kill(g_all.my_pid, signal))
// 	{
// 		g_all.status = 130;
// 		printf("Dans kill\n");
// 		exit(66);
// 	}
// 	else if (signal == SIGINT)// && pas dans un exec )
// 	{
// 		printf("BLOUuUUUUUUUUP\n");//******
// 		g_all.status = 1;
// 		// ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 		// rl_replace_line("", 0);
// 		// rl_on_new_line();
// 		printf("\e[2K");
//         rl_on_new_line();
//         rl_redisplay();
//         printf("\n");
//         rl_replace_line("", 0);
//         rl_on_new_line();
//         rl_redisplay();
// 	}

	// if (sign == SIGQUIT)
	// {
	// 	// ok ca ?
	// 	printf("\e[2K");
    //     rl_on_new_line();
    //     rl_redisplay();
	// }
// }

// void process_sign(int sign_id)
// {
// 	if (!kill(g_all.my_pid, sign_id))
// 	{
// 		printf("Dans kill\n");//******
// 	//	exit(66); // a suppr
// 		// if (sign_id == SIGQUIT)
// 		// {
// 		// 	ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
// 		// 	g_all.status = 131;
// 		// }
// 		// else if (sign_id == SIGINT)
// 		// {
// 		// 	ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 		// 	g_all.status = 130;
// 		// }
// 	}
// 	else if (sign_id == SIGINT)
// 	{
// 		g_all.status = 1;
// 		tputs(tgetstr("dl", NULL), 1, ft_putchar);
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		write(STDOUT_FILENO, "", 0);
// 	}
// }

// void	signal_handle(int sign_id)
// {
// 	if ((sign_id == SIGINT || sign_id == SIGQUIT) && g_all.my_pid != 0) // changer le pid
// 	{
// 		//printf("Ici\n");
// 		process_sign(sign_id);
// 	}
// 	// else
// 	// {
// 	// 	if (sign_id == SIGINT)
// 	// 	{
// 	// 		ft_putchar_fd('\n', STDOUT_FILENO);
// 	// 		g_all.status = 1;
// 	// 	}
// 	// 	else if (sign_id == SIGQUIT)
// 	// 		ft_putstr_fd("\b\b	\b\b", STDOUT_FILENO);	
// 	// }
// }