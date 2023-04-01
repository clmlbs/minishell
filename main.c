/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:24:37 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 13:12:16 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//t_context	g_all;

void	sign_ctrl_c(int sign)
{
	(void) sign;
}

int	check_line(char *input)
{
	if (!input)
	{
		printf("exit\n"); //fr en sorte qu'il n'affiche pas ^D
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
	// t_lexer	*buf;
	
	if (lexer(input) == FAILURE)
		return (FAILURE);
	if (parser() == FAILURE)
		return (FAILURE);
	print_t_lexer();//********
	if (fill_t_cmd() == FAILURE)
		return (FAILURE);
	// buf = g_all.lexer;
	// while (buf)
	// {

	// 	buf = buf->next;
	// }
}

void	init_global(char **av)
{
	(void)av;
	g_all.lexer = NULL;
	g_all.cmd = cmd_lstnew();
	if (!g_all.cmd)
		exit(1);// bon status de sortie ? 
	g_all.all_path = NULL;
	g_all.nb_cmd = 0;
}


int	main(int ac, char **av, char **env)
{
	char		*input;

	init_global(av);
	//env_check(env);
	if (ac != 1)
	{
		write_error("Error: ", "you cannot add arguments", \
		" after \"minishell\"\n");
		return (FAILURE);
	}
	while (1)
	{
		signal(SIGINT, sign_ctrl_c);
		//env_pwd_update(); // faut pas mettre ca apres l'exec ?
		input = readline(WATERMELON "Minishell " WHITE);
		if (check_line(input) == FAILURE)
			break ;
		if (check_line(input) != EMPTY)
		{
			g_all.nb_cmd = 1;
			minishell(input);
			ms_lstclear(&(g_all.lexer));
		}
		free(input);
	}
	//free les trucs de la globale 
	return (0);
}
