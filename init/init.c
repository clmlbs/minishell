/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:32:31 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/06 10:10:58 by cleblais         ###   ########.fr       */
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
	if (init_env(env) == FAILURE)
		exit(1); // bon statut?
	g_all.env = copy_strs_plus_one(env);
	if (!g_all.env)
		exit(1);// bon status de sortie ? 
	g_all.nb_cmd = 0;
	g_all.pid = NULL;
	g_all.fd_stdin = dup(STDIN_FILENO);
	g_all.fd_stdout = dup(STDOUT_FILENO);
	if (g_all.fd_stdin == -1 || g_all.fd_stdout == -1)
	{
		perror_fail("Minishell: dup()");
		exit(1); // bon statut ? 
	}
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

int	update_global(void)
{
	char	**new_env;
	int		*nb_strs;
	int		ssize;

	new_env = NULL;
	nb_strs = (int *)malloc(sizeof(int));
	if (!nb_strs)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: malloc()")));
	if (close(g_all.size[1]) < 0 || close(g_all.heritage[1]) < 0)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: close()")));
	ssize = read(g_all.size[0], nb_strs, sizeof(int));
	if (ssize == -1)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: read()")));
	if (*nb_strs <= 0) // ok ? 
		return (ret_upt(new_env, nb_strs, EMPTY));
	printf("nb_strs:%d\n", *nb_strs);//******
	new_env = (char **)malloc(sizeof(char *) * ((*nb_strs) + 1));
	ft_putstr_fd("Ici\n", 2);//******
	if (!new_env)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: malloc()")));
	if (read(g_all.heritage[0], new_env, sizeof(char *) * (*nb_strs)) == -1)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: read()")));
	free_tab_strs(g_all.env);
	g_all.env = new_env;
	new_env = NULL;
	return (ret_upt(new_env, nb_strs, SUCCESS));
}

int	ret_upt(char **new_env, int *nb_strs, int return_value)
{
	if (new_env)
		free(new_env);
	if (nb_strs)
		free(nb_strs);
	close(g_all.size[1]);
	close(g_all.size[0]);
	close(g_all.heritage[1]);
	close(g_all.heritage[0]);
	if (pipe(g_all.size) < 0)
		perror("Minishell: pipe()");
	if (pipe(g_all.heritage) < 0)
		perror("Minishell: pipe()");
	return (return_value);
}