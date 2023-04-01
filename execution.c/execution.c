/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:55:00 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 18:55:26 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_if_openable(t_cmd *cmd)
{
	DIR	*dir;
	int	fd;

	dir = opendir(cmd->infile_name);
	if (dir)
	{
		closedir(dir);
		write_error("Minishell: error: ", cmd->infile_name, " Is a directory\n");
		return (FAILURE);
	}
	if (access(cmd->infile_name, F_OK) == -1)
		return (ft_perror("Minishell: "));
	fd = open(cmd->infile_name, O_RDONLY);
	if (fd == -1)
		return (ft_perror("Minishell: "));
	close(fd);
}