/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:32:31 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/10 10:00:09 by cleblais         ###   ########.fr       */
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
		exit(FAILURE);
	}
	g_all.lexer = NULL;
	g_all.cmd = cmd_lstnew();
	if (!g_all.cmd)
		exit(FAILURE);
	if (init_env(env) == FAILURE)
		exit(FAILURE);
	g_all.env = copy_strs_plus_one(env);
	if (!g_all.env)
		exit(FAILURE); 
	if (update_shlvl() == FAILURE)
		exit(FAILURE);
	g_all.tilde = create_var_value("HOME", 0);
	if (!g_all.tilde)
		exit(FAILURE);
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
	// PENSER A FERMER LES FD !!!
}

int	init_env(char **envp)
{
	int	i;
	int	line;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			line = i;
			break ;
		}
		i++;
	}
	g_all.all_path = ms_split(envp[i] + 5, ':');
	if (!g_all.all_path)
		return (FAILURE);
	//printf_strs(g_all.all_path, WITH_INDEX, STDOUT_FILENO);//********
	return (SUCCESS);
}




int	ret_upt(char **new_env, int *nb_strs, int return_value)
{
	if (new_env)
		free(new_env);
	if (nb_strs)
		free(nb_strs);
	close(g_all.herit[1]);
	close(g_all.herit[0]);
	if (pipe(g_all.herit) < 0)
		perror("Minishell: pipe()");
	return (return_value);
}

void init_t_updated_token(t_update_token *t)
{
	t->i = 0;
	t->begin = 0;
	t->str_begin = NULL;
	t->var_name = NULL;
	t->var_value = NULL;
	t->begin_and_value = NULL;
	t->token_updated = NULL;
}