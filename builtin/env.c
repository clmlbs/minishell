/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 17:11:30 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 17:32:25 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_env(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->wd[1])
	{
		write_error("Minishell: ", "error: env should be executed ", \
		"without option nor argument\n");
		exit(2);
	}
	else
	{
		while (g_all.env[i])
		{
			if (ft_strchr(g_all.env[i], '=') != NULL)
			{
				if (ft_strncmp("PWD_TO_SUPPR=", g_all.env[i], 13))
				{
					ft_putstr_fd(g_all.env[i], cmd->fd_outfile);
					ft_putstr_fd("\n", cmd->fd_outfile);
				}
			}
			i++;
		}
		exit(SUCCESS);
	}
}
