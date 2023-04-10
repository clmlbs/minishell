/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 10:59:09 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/10 07:43:32 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*copy_t_cmd(t_cmd *src)
{
	t_cmd	*new;

	new = cmd_lstnew();
	if (!new)
		return (NULL);
	new->wd = copy_strs_plus_one(src->wd);
	if (!new->wd)
	{
		free_t_cmd(new);
		return (NULL);
	}
	new->fd_infile = src->fd_infile;
	new->infile_mode = src->infile_mode;
	new->fd_outfile = src->fd_outfile;
	new->outfile_mode = src->outfile_mode;
	new->position = src->position;
	if (copy_str_t_cmd(new, src) == FAILURE)
	{
		free_t_cmd(new);
		return (NULL);
	}
	return (new);
}

int	copy_str_t_cmd(t_cmd *new, t_cmd *src)
{
	if (src->infile_name)
	{
		new->infile_name = ft_strdup(src->infile_name);
		if (!new->infile_name)
			return (FAILURE);
	}
	if (src->outfile_name)
	{
		new->outfile_name = ft_strdup(src->outfile_name);
		if (!new->outfile_name)
			return (FAILURE);
	}
	if (src->good_path)
	{
		new->good_path = ft_strdup(src->good_path);
		if (!new->good_path)
			return (FAILURE);
	}
	return (SUCCESS);
}