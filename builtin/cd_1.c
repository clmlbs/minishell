/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_1.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 14:18:27 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/09 08:25:29 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_var_to_env(char *str)
{
	char	**buf;

	buf = copy_strs_plus_one(g_all.env);
	if (!buf)
		return (perror_fail("Minishell: malloc()"));
	free(buf[tab_strlen(g_all.env)]);//rajoute pour les leaks
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
			{
				free(var); // leaks
				return (FAILURE);
			}
		}
		j = 0;
	}
	free(var);//leaks
	return (SUCCESS);
}

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

int	replace_path(t_cmd *cmd)
{
	if (!cmd->wd[1] || (!ft_strcmp(cmd->wd[1], "--") && \
		ms_strlen(cmd->wd[1]) == 2))
		return (replace_home(cmd));
	if (!ft_strcmp(cmd->wd[1], "~") && ms_strlen(cmd->wd[1]) == 1)
		return (replace_tilde(cmd));
	if (!ft_strcmp(cmd->wd[1], "-") && ms_strlen(cmd->wd[1]) == 1)
		return (replace_oldpwd(cmd));
	if (cmd->wd[1][0] == '-' && ms_strlen(cmd->wd[1]) != 1)
	{
		ft_putstr_fd("Minishell: cd: ", 2);
		ft_putstr_fd(cmd->wd[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		return (FAILURE);
	}
	return (SUCCESS);
}

void	execute_cd(t_cmd *cmd)
{
	int	result;
	int	i;

	i = 0;
	print_t_cmd(cmd);//*********
	result = replace_path(cmd);
	if (result != SUCCESS)
		exit (result);//verif les codes 
	if (check_destination(cmd) == FAILURE)
		exit (FAILURE);
	if (add_oldpwd(cmd) == FAILURE) 
		exit(FAILURE);
	if (chdir(cmd->wd[1]) == -1)
	{
		perror("Minishell");
		exit(FAILURE);
	}
	//======= RETIRER ==========
	char	cwd[1024];
	getcwd(cwd, 1024);
	printf("pwd:%s\n", cwd);//********

	if (add_newpwd(cmd) == FAILURE)
		exit(FAILURE);
	if (send_env_to_father(g_all.env, g_all.herit) == FAILURE)
		exit(FAILURE);
	exit(SUCCESS);
}