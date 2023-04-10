/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 07:41:19 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/10 11:27:54 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <curses.h>
#include <term.h>

void	signal_handle(int sig)
{
	if (sig == SIGINT)// && pas dans un exec )
	{
		g_all.status = 1; // ici doit etre 130 quand dans un prg 
		// ioctl(STDIN_FILENO, TIOCSTI, "\n");
		// rl_replace_line("", 0);
		// rl_on_new_line();
		printf("\e[2K");
        rl_on_new_line();
        rl_redisplay();
        printf("\n");
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		// ok ca ?
		printf("\e[2K");
        rl_on_new_line();
        rl_redisplay();
	}
}


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