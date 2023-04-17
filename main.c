/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:24:37 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/17 11:14:24 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_line(char *input)
{
	if (!input)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		printf("exit\n");
		return (FAILURE);
	}
	if (input[0] != '\0')
	{
		add_history(input);
		return (SUCCESS);
	}
	else
		return (EMPTY);
}

void	minishell(char *input)
{
	if (lexer(input) == FAILURE)
	{
		if (g_all.status == 0)
			g_all.status = 1;
		return ;
	}
	if (parser() == FAILURE)
	{
		if (g_all.status == 0)
			g_all.status = 258;
		return ;
	}
	if (fill_t_cmd() == FAILURE)
	{
		if (g_all.status == 0)
			g_all.status = 1;
		return ;
	}
	if (execute_line() == FAILURE)
	{
		if (g_all.status == 0)
			g_all.status = 1;
		return ;
	}
}

int	prompt(void)
{
	char		*input;
	int			line_ok;

	//system("leaks minishell"); //********
	echo_ctl(0);
	signal(SIGINT, signal_handle);
	signal(SIGQUIT, signal_handle);
	if (g_all.is_first_turn == NO && isatty(STDIN_FILENO) \
		&& update_global() == FAILURE)
		return (FAILURE);
	g_all.is_first_turn = YES;
	input = readline(WATERMELON "Minishell " WHITE);
	line_ok = check_line(input);
	if (line_ok == FAILURE)
		return (FAILURE);
	if (line_ok != EMPTY)
	{
		g_all.nb_cmd = 1;
		minishell(input);
		free_all_lexer_and_cmd();
		g_all.is_first_turn = NO;
	}
	free(input);
	return (SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	init_global(ac, av, env);
	if (pipe(g_all.herit) < 0)
		return (perror_fail("Minishell: pipe()"));
	printf("1er pid:%d\n", getpid());//****** A SUPPRIMER
	while (1)
	{
		if (prompt() == FAILURE) // verif que pas de double free
			break ;
	}
	free_everything();
	close(g_all.fd_stdin);
	close(g_all.fd_stdout);
	//system("leaks minishell"); //********
	//free les trucs de la globale 
	// close herit + size
	return (0);
}
