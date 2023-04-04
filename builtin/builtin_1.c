/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 08:44:20 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/04 09:16:47 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(t_cmd *cmd)
{
	// if (ft_strlen(cmd->wd[0]) == 2 && !ft_strncmp(cmd->wd[0], "cd", 2))
	// 	return (TRUE);
	/*else*/ if (ft_strlen(cmd->wd[0]) == 3 && !ft_strncmp(cmd->wd[0], "pwd", 3))
		return (TRUE);
	else if (ft_strlen(cmd->wd[0]) == 3 && !ft_strncmp(cmd->wd[0], "env", 3))
		return (TRUE);
	// else if (ft_strlen(cmd->wd[0]) == 4 && !ft_strncmp(cmd->wd[0], "echo", 4))
	// 	return (TRUE);
	// else if (ft_strlen(cmd->wd[0]) == 4 && !ft_strncmp(cmd->wd[0], "exit", 4))
	// 	return (TRUE);
	// else if (ft_strlen(cmd->wd[0]) == 5 && !ft_strncmp(cmd->wd[0], "unset", 5))
	// 	return (TRUE);
	// else if (ft_strlen(cmd->wd[0]) == 6 && !ft_strncmp(cmd->wd[0], "export", 6))
	// 	return (TRUE);
	else
		return (FALSE);
}

void	execute_env(t_cmd *cmd)
{
	if (cmd->wd[1])
	{
		write_error("Minishell: ", "error: env should be executed ",\
		"without option nor argument\n");
		exit(FAILURE);
	}
	else
	{
		printf_strs(g_all.env, WITHOUT_INDEX, cmd->fd_outfile);
		// mettre le bon status
		exit(SUCCESS);
	}
}

void	execute_pwd(void)
{
	char	cwd[1024];
	
	if (!getcwd(cwd, 1024))
	{
		perror(ORANGE "Minishell: getcwd()" WHITE);
		exit(FAILURE);
	}
	printf("%s\n", cwd);
	exit(SUCCESS);
}