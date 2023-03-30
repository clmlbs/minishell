/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:32:31 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 10:47:07 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_t_cmd(t_cmd *cmd)
{
	// cmd = (t_cmd *)malloc(sizeof(t_cmd));
	// if (!cmd)
	// 	return(ft_perror("Minishell: malloc()"));
	// g_ctx.cmd = cmd;
	cmd->wd = NULL;
	cmd->infile_name = NULL;
	cmd->outfile_name = NULL;
	cmd->good_path = NULL;
	return (SUCCESS);
}