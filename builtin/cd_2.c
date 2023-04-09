/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 08:27:50 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/09 08:28:32 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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