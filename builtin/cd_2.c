/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 08:27:50 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/09 12:51:16 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
extern t_all g_all;

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
	
	var = create_var_value("HOME", 0);
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

	var = create_var_value("OLDPWD", 0);
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
	ft_putstr_fd(cmd->wd[1], cmd->fd_outfile);
	ft_putstr_fd("\n", cmd->fd_outfile);
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

int	add_newpwd(t_cmd *cmd)
{
	char	*value;
	char	*var;
	char	cwd[1024];

	if (!getcwd(cwd, 1024))
	{
		perror("Minishell: getcwd()");
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