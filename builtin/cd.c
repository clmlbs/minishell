/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 14:18:27 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/08 16:43:40 by cleblais         ###   ########.fr       */
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

int	replace_tilde(t_cmd *cmd)
{
	
	cmd->wd[1] = ft_strdup(g_all.tilde);
	if (!cmd->wd[1])
	{
		perror("Minishell");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	replace_home(t_cmd *cmd)
{
	char 	*var;
	
	var = create_var_value("HOME");
	if (!var)
		return (FAILURE);
	if (!var[0])
	{
		ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
		free(var);
		return (EMPTY);
	}
	free(cmd->wd[1]);
	cmd->wd[1] = var;
	return (SUCCESS);
}

int	replace_oldpwd(t_cmd *cmd)
{
	char *var;
	
	var = create_var_value("OLDPWD");
	if (!var)
		return (FAILURE);
		printf("var:%s\n", var);//******
	if (!var[0])
	{
		ft_putstr_fd("Minishell: cd: OLDPWD not set\n", 2);
		free(var);
		return (EMPTY);
	}
	free(cmd->wd[1]);
	cmd->wd[1] = var;
	return (SUCCESS);
}

int	replace_path(t_cmd *cmd)
{
	int	result;
// check si --- ou -option quelconque
	if (!cmd->wd[1] || (!ft_strcmp(cmd->wd[1], "--") && ft_strlen(cmd->wd[1]) == 2))
	{
		result = replace_home(cmd);
		if (result != SUCCESS)
			return (result);
	}
	if (!ft_strcmp(cmd->wd[1], "~") && ft_strlen(cmd->wd[1]) == 1)
	{
		result = replace_tilde(cmd);
		if (result != SUCCESS)
			return (result);
	}
	if (!ft_strcmp(cmd->wd[1], "-") && ft_strlen(cmd->wd[1]) == 1)
	{
		printf("Ici\n");//********
		result = replace_oldpwd(cmd);
		if (result != SUCCESS)
			return (result);
	}
	if (cmd->wd[1][0] == '-' && ft_strlen(cmd->wd[1]) != 1)
	{
		ft_putstr_fd("Minishell: cd: ", 2);
		ft_putstr_fd(cmd->wd[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	check_destination(t_cmd *cmd)
{
	DIR	*dir;
	
	dir = opendir(cmd->wd[1]);
	if (dir == NULL)
	{
		ft_putstr_fd("Minishell: cd: ", 2);
		ft_putstr_fd(cmd->wd[1], 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (FAILURE);
	}
	closedir(dir);
	return (SUCCESS);
}

// penser a verifier que les message d'erreur se font quand 
// on essaie d'aller dans un drectory qui na pas ete set - ou ~ ou --

void	execute_cd(t_cmd *cmd)
{
	int	result;

	if (add_oldpwd(cmd) == FAILURE) 
		exit(FAILURE);
	result = replace_path(cmd);
	if (result != SUCCESS)
		exit (result);//verif les codes 
	if (check_destination(cmd) == FAILURE)
		exit (FAILURE);
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