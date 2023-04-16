/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 17:07:47 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 17:41:45 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	getcwd_doesnt_work(t_cmd *cmd)
{
	char	*str;

	str = NULL;
	str = create_var_value("PWD");
	if (!str)
	{
		str = create_var_value("PWD_TO_SUPPR");
		if (!str)
		{
			perror("Minishell");
			exit(FAILURE);
		}
	}
	ft_putstr_fd(str, cmd->fd_outfile);
	ft_putstr_fd("\n", cmd->fd_outfile);
	if (str)
		free(str);
	exit(SUCCESS);
}

void	execute_pwd(t_cmd *cmd)
{
	char	cwd[1024];

	if (cmd->wd[1] && cmd->wd[1][0] == '-')
	{
		write_error("Minishell: ", "error: pwd should be executed ", \
		"without option\n");
		exit(2);
	}
	getcwd(cwd, 1024);
	if (!cwd[0])
		getcwd_doesnt_work(cmd);
	ft_putstr_fd(cwd, cmd->fd_outfile);
	ft_putstr_fd("\n", cmd->fd_outfile);
	exit(SUCCESS);
}
