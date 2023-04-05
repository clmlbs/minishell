/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:32:31 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/05 12:59:04 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_global(int ac, char **av, char **env)
{
	(void)av;
	if (ac != 1)
	{
		write_error("Error: ", "you cannot add arguments", \
		" after \"minishell\"\n");
		exit(1); // bon status ?
	}
	g_all.lexer = NULL;
	g_all.cmd = cmd_lstnew();
	if (!g_all.cmd)
		exit(1);// bon status de sortie ? 
	g_all.all_path = NULL;
	g_all.env = copy_strs_plus_one(env);
	if (!g_all.env)
		exit(1);// bon status de sortie ? 
	g_all.nb_cmd = 0;
	g_all.pid = NULL;
	g_all.fd_stdin = dup(STDIN_FILENO); // mettre les protections
	g_all.fd_stdout = dup(STDOUT_FILENO); // mettre les protections 
	// PENSER A FERMER LES FD !!!
}

