/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 18:41:46 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/06 13:55:51 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset_var(char ***new, t_cmd *cmd, int *index, int *var_target)
{
	(*new) = copy_strs_plus_one(g_all.env);
	if (!(*new))
		return (FAILURE);
	if (is_var_exist(cmd->wd[*var_target], index) == SUCCESS)
	{
		(*new) = remove_var(*new, *index, 0);
		if (!(*new))
			return (FAILURE);
		free_tab_strs(g_all.env);
		g_all.env = (*new);
		(*new) = NULL;
	}
	(*index) = 0;
	(*var_target)++;
	return (SUCCESS);
}


void	execute_unset(t_cmd	*cmd)
{
	char	**new;
	int		var_target;
	int		index;

	var_target = 1;
	index = 0;
	while (cmd->wd[var_target])
	{
		if (cmd->wd[var_target] && cmd->wd[var_target][0] == '-')
		{
			write_error("Minishell: ", "options for unset are not ", \
				"implemented in this Minishell\n");
			var_target++;
		}
		else
		{
			if (unset_var(&new, cmd, &index, &var_target) == FAILURE)
				exit(FAILURE);
		}
	}
	if (new)
		free_tab_strs(new);
	if (send_env_to_father(g_all.env, g_all.herit) == FAILURE)
		exit(FAILURE);
	exit(SUCCESS);
}

char	**remove_var(char **env, int index, int i)
{
	char	**new;

	new = (char **)malloc(sizeof(char *) * tab_strlen(env) + 1);
	if (!new)
		return (error_strdup(env, new));
	while (env[i] && i < index)
	{
		new[i] = ft_strdup(env[i]);
		if (!new[i])
			return (error_strdup(env, new));
		i++;
	}
	while (env[i + 1])
	{
		new[i] = ft_strdup(env[i + 1]);
		if (!new[i])
			return (error_strdup(env, new));
		i++;
	}
	new[i] = NULL;
	free_tab_strs(env);
	return (new);
}

char	**error_strdup(char **env, char **new)
{
	perror("Minishell: malloc()");
	free_tab_strs(env);
	if (new)
		free_tab_strs(new);
	return (NULL);
}