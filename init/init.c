/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:32:31 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/15 13:58:40 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	update_shlvl(void)
{
	char	*shlvl;
	char	*value_str;
	int		value_int;
	char 	*str_joined;

	shlvl = ft_strdup("SHLVL");
	if (!shlvl)
		return (FAILURE);
	value_str = create_var_value(shlvl);
	if (!value_str)
		return (FAILURE);
	free(shlvl);
	value_int = ms_atoi(value_str) + 1;
	free(value_str);
	value_str = ft_itoa(value_int);
	if (!value_str)
		return (FAILURE);
	str_joined = ms_strjoin("SHLVL=", value_str);
	free(value_str);
	if (!str_joined)
		return (FAILURE);
	if (add_or_update_var_in_env(str_joined) == FAILURE)
	{
		free(str_joined);
		return (FAILURE);
	}
	free(str_joined);
	return (SUCCESS);
}

int	remove_var_without_egal(void)
{
	int		i;
	char	**new;

	i = 0;
	while (g_all.env[i])
	{
		if (ft_strchr(g_all.env[i], '=') == NULL)
		{
			new = copy_strs_plus_one(g_all.env);
			if (!new)
				return (FAILURE);
			new = remove_var(new, i, 0);
			if (!new)
				return (FAILURE); // besoin de passer par un buf comme unset var ?
			free_tab_strs(g_all.env);
			g_all.env = new;
			new = NULL;		
		}
		else
			i++;
	}
	return (SUCCESS);
}

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
	if (save_all_path(copy_strs_plus_one(env)) == FAILURE)
		exit(FAILURE);
	g_all.env = copy_strs_plus_one(env);
	if (!g_all.env)
		exit(FAILURE); 
	if (update_shlvl() == FAILURE)
		exit(FAILURE);
	g_all.tilde = create_var_value("HOME");
	if (!g_all.tilde)
		exit(FAILURE);
	if (remove_var_without_egal() == FAILURE)
		exit(FAILURE);
	g_all.nb_cmd = 0;
	g_all.pid = NULL;
	g_all.my_pid = 0;// ok ?
	g_all.where = PROCESS;
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

int	save_all_path(char **envp)
{
	int	i;

	i = 0;
	if (g_all.all_path)
		free_tab_strs(g_all.all_path);
	g_all.all_path = NULL;
	if (!envp)
		return (FAILURE);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			g_all.all_path = ms_split(envp[i] + 5, ':');
			free_tab_strs(envp);
			if (!g_all.all_path)
				return (FAILURE);
			return (SUCCESS);
		}
		i++;
	}
	free_tab_strs(envp);
	return (SUCCESS);
}

// int	update_global(void)
// {
// 	char	**new_env;
// 	int		*nb_strs;
// 	size_t	ssize;
// 	int		i;
// 	size_t	len;
	
// 	new_env = NULL;
// 	nb_strs = (int *)malloc(sizeof(int));
// 	if (!nb_strs)
// 		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: malloc()")));
// 	if (close(g_all.size[1]) < 0 || close(g_all.herit[1]) < 0)
// 		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: close()")));
	
// 	ssize = read(g_all.size[0], nb_strs, sizeof(int));
	
// 	if (ssize == -1)
// 		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: read()")));
// 	if (*nb_strs <= 0) // ok ? 
// 		return (ret_upt(new_env, nb_strs, EMPTY));
// 	new_env = (char **)malloc(sizeof(char *) * ((*nb_strs) + 1));
// 	if (!new_env)
// 		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: malloc()")));
// 	i = 0;

// 	while (i < *nb_strs)
// 	{
// 		len = 0;
// 		if (read(g_all.herit[0], &len, sizeof(size_t)) == -1)
// 			return (ret_upt(new_env, nb_strs, perror_fail("Minishell: read()")));
// 		printf("len:%zu\n", len);
// 		new_env[i] = (char *)malloc(len);
// 		if (!new_env[i])
// 		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: malloc()")));
// 		if (read(g_all.herit[0], new_env[i], len) == -1)
// 			return (ret_upt(new_env, nb_strs, perror_fail("Minishell: read()")));
// 		i++;
// 	}
// 	printf("ici\n");//*********
// 	new_env[*nb_strs] = NULL;
// 	printf_strs(new_env, WITH_INDEX, 2);//*********
// 	free_tab_strs(g_all.env);
// 	g_all.env = new_env;
// 	new_env = NULL;
// 	return (ret_upt(new_env, nb_strs, SUCCESS));
// }

int	update_global(void)
{
	char	**new_env;
	int		*nb_strs;
	size_t	ssize;
	int		i;
	size_t	len;

	new_env = NULL;
	nb_strs = (int *)malloc(sizeof(int));
	if (!nb_strs)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: malloc()")));
	if (close(g_all.herit[1]) < 0)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: close()")));
	ssize = read(g_all.herit[0], nb_strs, sizeof(int));
	if (ssize == -1)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: read()")));
	if (*nb_strs <= 0)
		return (ret_upt(new_env, nb_strs, EMPTY));
	new_env = (char **)malloc(sizeof(char *) * ((*nb_strs) + 1));
	if (!new_env)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: malloc()")));
	i = 0;
	while (i < *nb_strs)
	{
		len = 0;
		if (read(g_all.herit[0], &len, sizeof(size_t)) == -1)
			return (ret_upt(new_env, nb_strs, perror_fail("Minishell: read()")));
		new_env[i] = (char *)malloc(len);
		if (!new_env[i])
			return (ret_upt(new_env, nb_strs, perror_fail("Minishell: malloc()")));
		if (read(g_all.herit[0], new_env[i], len) == -1)
			return (ret_upt(new_env, nb_strs, perror_fail("Minishell: read()")));
		i++;
	}
	new_env[*nb_strs] = NULL;
	free_tab_strs(g_all.env);
	g_all.env = new_env;
	new_env = NULL;
	if (save_all_path(copy_strs_plus_one(g_all.env)) == FAILURE)
		return (ret_upt(new_env, nb_strs, FAILURE)); 
	return (ret_upt(new_env, nb_strs, SUCCESS));
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