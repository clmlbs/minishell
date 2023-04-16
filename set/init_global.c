/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_global.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 21:36:37 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 21:42:04 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_all_int_and_null(char **av)
{
	(void)av;
	g_all.lexer = NULL;
	g_all.nb_cmd = 0;
	g_all.pid = NULL;
	g_all.my_pid = 0;
	g_all.fd_stdin = dup(STDIN_FILENO);
	g_all.fd_stdout = dup(STDOUT_FILENO);
	if (g_all.fd_stdin == -1 || g_all.fd_stdout == -1)
	{
		perror_fail("Minishell: dup()");
		exit(FAILURE);
	}
	g_all.is_first_turn = YES;
	g_all.status = 0;
	g_all.where = PROCESS;
}

void	init_global(int ac, char **av, char **env)
{
	if (ac != 1)
	{
		write_error("Error: ", "you cannot add arguments", \
		" after \"minishell\"\n");
		exit(FAILURE);
	}
	init_all_int_and_null(av);
	g_all.cmd = cmd_lstnew();
	if (!g_all.cmd)
		exit(FAILURE);
	if (save_all_path(copy_strs_plus_one(env)) == FAILURE)
		exit(FAILURE);
	g_all.env = copy_strs_plus_one(env);
	if (!g_all.env)
		exit(FAILURE);
	if (update_shlvl_part_1() == FAILURE)
		exit(FAILURE);
	g_all.tilde = create_var_value("HOME");
	if (!g_all.tilde)
		exit(FAILURE);
	if (remove_var_without_egal() == FAILURE)
		exit(FAILURE);
	if (remove_oldpwd() == FAILURE)
		exit(FAILURE);
}
