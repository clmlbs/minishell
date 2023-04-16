/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 17:10:14 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 17:10:34 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_echo(t_cmd *cmd, int i)
{
	int	j;
	
	if (cmd->wd[1] == NULL)
	{
		ft_putstr_fd("\n", cmd->fd_outfile);
		exit(SUCCESS);
	}
	if (ms_strlen(cmd->wd[1]) >= 2 && !ft_strncmp("-n", cmd->wd[1], 2))
	{
		i++;
		if (!cmd->wd[2])
				exit(SUCCESS) ;
		while (cmd->wd[i] && ms_strlen(cmd->wd[i]) >= 2 && !ft_strncmp("-n", cmd->wd[i], 2))
		{
			j = 2;
			while (cmd->wd[i][j] == 'n')
				j++;
			if (ms_strlen(cmd->wd[i]) == j)
				i++;
			else
				break ;
		}
	}
	while (cmd->wd[i + 1])
	{
		ft_putstr_fd(cmd->wd[i], cmd->fd_outfile);
		ft_putstr_fd(" ", cmd->fd_outfile);
		i++;
	}
	ft_putstr_fd(cmd->wd[i], cmd->fd_outfile);
	if (!(ms_strlen(cmd->wd[1]) >= 2 && !ft_strncmp("-n", cmd->wd[1], 2)))
		ft_putstr_fd("\n", cmd->fd_outfile);
	exit(SUCCESS);
}