/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 15:12:12 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/11 14:53:09 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	signal_main(int signal)
// {
// 	(void)signal;
// 	ft_putstr_fd("\n", STDOUT_FILENO);
// 	if (g_all.my_pid)
// 		kill(g_all.my_pid, SIGINT);
// 	else
// 	{
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// 	g_all.my_pid = 0;
// }


void	signal_main(int signal)
{
	if (signal == SIGINT && g_all.where == DAD)
	{
		//printf("DANS DAD\n");//*****
		g_all.status = 1;
		printf("\e[2K"); // to clear the entire line where the cursor is currently located.
        rl_on_new_line();
        rl_redisplay();
        printf("\n");
        rl_replace_line("", 0); //replace the current contents of the Readline input buffer with a new string. 
        rl_on_new_line(); // to move the cursor to the beginning of a new line.
        rl_redisplay(); // to update the display of the current line being edited by Readline.
	}
	else if (signal == SIGINT && g_all.where == SON)
	{
		//printf("DANS SON\n");//*******
		g_all.status = 130;
		printf("^C\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	else if (signal == SIGINT && g_all.where == HERE_DOC)
	{
		//printf("DANS HEREDOC\n");//******
		(void)signal;
		if (g_all.my_pid == 0)
			exit(130);
		else
		{
			//ft_putstr_fd("\b\b  \b\b", 1);
			rl_redisplay();
			printf("\n");
		}
		g_all.status = 130; // ou 1 ?
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