/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env_after_son.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 21:23:34 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 22:10:21 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	read_env_of_son(char ***new_env, int **nb_strs)
{
	size_t	len;
	int		i;

	i = 0;
	while (i < **nb_strs)
	{
		len = 0;
		if (read(g_all.herit[0], &len, sizeof(size_t)) == -1)
			return (ret_upt(*new_env, *nb_strs, \
				perror_fail("Minishell: read()")));
		(*new_env)[i] = (char *)malloc(len);
		if (!(*new_env)[i])
			return (ret_upt(*new_env, *nb_strs, \
				perror_fail("Minishell: malloc()")));
		if (read(g_all.herit[0], (*new_env)[i], len) == -1)
			return (ret_upt(*new_env, *nb_strs, \
				perror_fail("Minishell: read()")));
		i++;
	}
	return (SUCCESS);
}

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
	if (*nb_strs <= 0 || *nb_strs > 1000) // car quand on n'a pas rempli le pipe system leaks lit des grand nb pos et sas rien ils sont neg
		return (ret_upt(new_env, nb_strs, EMPTY));
	new_env = (char **)malloc(sizeof(char *) * ((*nb_strs) + 1));
	if (!new_env)
		return (ret_upt(new_env, nb_strs, perror_fail("Minishell: malloc()")));
	if (read_env_of_son(&new_env, &nb_strs) == FAILURE)
		return (FAILURE);
	new_env[*nb_strs] = NULL;
	free_tab_strs(g_all.env);
	g_all.env = new_env;
	new_env = NULL;
	if (save_all_path(copy_strs_plus_one(g_all.env)) == FAILURE)
		return (ret_upt(new_env, nb_strs, FAILURE)); 
	if (cd_new_pwd() == FAILURE)
		return (ret_upt(new_env, nb_strs, FAILURE));
	return (ret_upt(new_env, nb_strs, SUCCESS));
}

int	cd_new_pwd(void)
{
	char	*pwd;
	int		i;
	char	**new_env;
	int		index;

	i = 0;
	if (g_all.is_first_turn == NO && is_var_exist("PWD", &i) == SUCCESS)
	{
		pwd = create_var_value("PWD");
		if (!pwd)
			return (FAILURE);
		chdir(pwd);
		free(pwd);
	}
	i = 0;
	if (g_all.is_first_turn == NO && is_var_exist("PWD_TO_SUPPR", &i) == 0)
	{
		index = 0;
		pwd = create_var_value("PWD_TO_SUPPR");
		if (!pwd)
			return (FAILURE);
		chdir(pwd);
		free(pwd);
	}
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
