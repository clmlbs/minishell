/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 14:18:27 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/08 00:10:09 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_var_to_env(char *str)
{
	char	**buf;

	buf = copy_strs_plus_one(g_all.env);
	if (!buf)
		return (perror_fail("Minishell: malloc()"));
	buf[tab_strlen(g_all.env)] = ft_strdup(str);
	if (!buf[tab_strlen(g_all.env)])
		return (perror_fail("Minishell: malloc()"));
	free_tab_strs(g_all.env);
	g_all.env = buf;
	buf = NULL;
	return (SUCCESS);
}

int	add_or_update_var_in_env(char *var_str)
{
	int		j;
	char	*var;

	j = 0;
	if (var_str)
	{
		var = ft_substr(var_str, 0, var_name_len(var_str));
		if (!var)
			return (FAILURE);
		else if (is_var_exist(var, &j) == SUCCESS)
			change_var_value(var_str, &j);
		else
		{
			if (add_var_to_env(var_str) == FAILURE)
				return (FAILURE);
		}
		j = 0;
	}
	return (SUCCESS);
}

// int	cd_update_env(t_cmd *cmd)
// {
// 	char	*value;
// 	char	*var;

// 	value = create_var_value("OLDPWD");
// 	if (!value)
// 		return (FAILURE);
// 	var = ms_strjoin("OLDPWD=", value);
// 	if (!var)
// 		return (FAILURE);
// 	if (add_or_update_var_in_env(var) == FAILURE)
// 		return (FAILURE);
// 	free(value);
// 	free(var);
// 	if (!cmd->wd[1])
// 	{
// 		if (cmd->wd[1] = creat_var_value(g_all.tilde) == -1)
// 		{
// 			perror("Minishell");
// 			return (FAILURE);
// 		}
// 	}
// 	value = ft_strdup(cmd->wd[1]);
// 	if (!value)
// 		return (FAILURE);
// 	var = ms_strjoin("PWD=", value);
// 	if (!var)
// 		return (FAILURE);
// 	if (add_or_update_var_in_env(var) == FAILURE)
// 		return (FAILURE);
// 	free(value);
// 	free(var);
// 	return (SUCCESS);
// }

int	add_oldpwd(t_cmd *cmd)
{
	char	*value;
	char	*var;

	value = create_var_value("PWD");
	if (!value)
		return (FAILURE);
	var = ms_strjoin("OLDPWD=", value);
	if (!var)
		return (FAILURE);
	if (add_or_update_var_in_env(var) == FAILURE)
		return (FAILURE);
	free(value);
	free(var);
	return (SUCCESS);
}

int	add_newpwd(t_cmd *cmd)
{
	char	*value;
	char	*var;
	char	cwd[1024];

	if (!getcwd(cwd, 1024))
	{
		perror(ORANGE "Minishell: getcwd()" WHITE);
		exit(FAILURE);
	}
	value = ft_strdup(cwd);
	if (!value)
		return (FAILURE);
	var = ms_strjoin("PWD=", value);
	if (!var)
		return (FAILURE);
	if (add_or_update_var_in_env(var) == FAILURE)
		return (FAILURE);
	free(value);
	free(var);
	return (SUCCESS);
}

int	check_destination(t_cmd *cmd) // ok ou passer par open ?
{
	DIR *dir;

	if (!cmd->wd[1])
	{
		cmd->wd[1] = create_var_value(g_all.tilde);
		if (!cmd->wd[1])
		{
			perror("Minishell");
			return (FAILURE);
		}
	}
	dir = opendir(cmd->wd[1]);
	if (dir == NULL)
	{
		perror("Minishell");
		return (FAILURE);
	}
	closedir(dir);
	return (SUCCESS);
}

void	execute_cd(t_cmd *cmd)
{
	int	result;

	if (add_oldpwd(cmd) == FAILURE) 
		exit(FAILURE);
	result = check_destination(cmd);
	if (result != SUCCESS)
		exit(result);
	if (chdir(cmd->wd[1]) == -1)
	{
		perror("Minishell");
		exit(FAILURE);
	}
	if (add_newpwd(cmd) == FAILURE)
		exit(FAILURE);
	if (send_env_to_father(g_all.env, g_all.herit) == FAILURE)
		exit(FAILURE);
	exit(SUCCESS);
}