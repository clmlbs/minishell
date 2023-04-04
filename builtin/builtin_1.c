/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 08:44:20 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/04 09:03:51 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_env(t_cmd *cmd)
{
	if (cmd->wd[1])
	{
		write_error("Minishell: ", "error: env should be executed ",\
		"without option nor argument\n");
		// mettre bon status
	}
	else
	{
		printf_strs(g_all.env, WITHOUT_INDEX, cmd->fd_outfile);
		// mettre le bon status
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