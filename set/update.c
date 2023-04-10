/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 07:46:05 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/10 07:49:36 by cleblais         ###   ########.fr       */
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
	value_str = create_var_value(shlvl, 0);
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
	if (update_env_after_son() == FAILURE) /// OK CAA ???
		return (ret_upt(new_env, nb_strs, FAILURE)); // OK ca ?? 
	return (ret_upt(new_env, nb_strs, SUCCESS));
}

int	update_env_after_son(void)
{
	char	*pwd;

	if (g_all.is_first_turn == NO)
	{
		pwd = create_var_value("PWD", 0);
		if (!pwd)
			return (FAILURE);
		if (chdir(pwd) == -1)
		{
			perror("Minishell");
			free(pwd);
			return (FAILURE);
		}
		free(pwd);
	}
	return (SUCCESS);
}