/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:24:37 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 16:42:28 by cleblais         ###   ########.fr       */
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
		return(SUCCESS);
	}
	else
		return (EMPTY);
}

int	main(int arc, char **arv, char **env)
{
	char		*input;

	(void) arv;
	(void) arc;
	//env_check(env);
	if (arc != 1)
	{
		write_error("Error: ", "you cannot add arguments",\
		" after \"minishell\"\n");
		return (FAILURE);
	}
	while (1)
	{
		signal(SIGINT, sign_ctrl_c);
		//env_pwd_update(); // faut pas mettre ca apres l'exec ?
		input = readline(WATERMELON "prompt " WHITE);
		if (check_line(input) == FAILURE)
			break ;
		if (check_line(input) != EMPTY)
		{
			g_all.nb_cmd = 1;
			lexer(input);
			ms_lstclear(&(g_all.lexer));
		}
		free(input);
	}
	//free les trucs de la globale 
	return (0);
}
