/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 12:01:56 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/03 14:14:57 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_good_path(t_cmd *cmd)
{
	int		i;

	i = 0;
	while (g_all.all_path[i])
	{
		cmd->good_path = ms_strjoin(g_all.all_path[i], cmd->wd[0]);//ms au lieu de ft
		if (!cmd->good_path)
			return (FAILURE);
		if (access(cmd->good_path, X_OK) == 0)
			return (ft_perror("Minishell: access()"));
		i++;
	}
	//write_error("Minishell", lst->wd[0], ": command not found\n"); normalement pas besoin car ft_perror 
	return (SUCCESS);
}


int	execute_child(t_cmd *cmd)
{
	if (find_good_path(cmd) == FAILURE)
		return (FAILURE);
	// if (dup_fd(cmd) == FAILURE)
	// 	return (FAILURE);
	print_t_cmd();//***** A SUPPR
	if (execve(cmd->good_path, cmd->wd, g_all.env) == -1)
		return (ft_perror("Minishell: execve()"));
	return (SUCCESS);
}