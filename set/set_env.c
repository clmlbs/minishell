/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:32:31 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 21:37:08 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	update_shlvl_part_1(void)
{
	char	*shlvl;
	char	*value_str;
	int		value_int;

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
	return (update_shlvl_part_2(value_str));
}

int	update_shlvl_part_2(char *value_str)
{
	char	*str_joined;

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
				return (FAILURE);
			free_tab_strs(g_all.env);
			g_all.env = new;
			new = NULL;
		}
		else
			i++;
	}
	return (SUCCESS);
}

int	remove_oldpwd(void)
{
	int		i;
	char	**new;

	i = 0;
	while (g_all.env[i])
	{
		if (!ft_strncmp(g_all.env[i], "OLDPWD=", 7))
		{
			new = copy_strs_plus_one(g_all.env);
			if (!new)
				return (FAILURE);
			new = remove_var(new, i, 0);
			if (!new)
				return (FAILURE);
			free_tab_strs(g_all.env);
			g_all.env = new;
			new = NULL;
		}
		else
			i++;
	}
	return (SUCCESS);
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
