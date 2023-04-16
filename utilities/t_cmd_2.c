/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 10:59:09 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 22:47:15 by cleblais         ###   ########.fr       */
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

void	print_t_cmd(t_cmd *cmd)
{
	t_cmd	*buf;

	if (cmd == NULL)
		buf = g_all.cmd;
	else
		buf = cmd;
	while (buf)
	{
		printf_strs(buf->wd, WITH_INDEX, STDOUT_FILENO);
		printf("fd_infile	%d\n", buf->fd_infile);
		printf("fd_outfile	%d\n", buf->fd_outfile);
		printf("I_mode:		%d\n", buf->infile_mode);
		printf("O_mode:		%d\n", buf->outfile_mode);
		printf("I_name:		%s\n", buf->infile_name);
		printf("O_name: 	%s\n", buf->outfile_name);
		printf("good_path:	%s\n", buf->good_path);
		printf("position:	%d\n\n", buf->position);
		buf = buf->next;
	}
}

char	**copy_strs_plus_one(char **old)
{
	int		tab_len;
	int		i;
	char	**new;

	tab_len = tab_strlen(old);
	new = (char **)malloc(sizeof(char *) * (tab_len + 2));
	if (!new)
		perror_fail("Minishell: malloc()");
	if (!new)
		return (NULL);
	i = -1;
	while (++i < tab_len)
	{
		new[i] = ft_strdup(old[i]);
		if (!new[i])
		{
			free_tab_strs(new);
			return (NULL);
		}
	}
	new[tab_len] = NULL;
	new[tab_len + 1] = NULL;
	return (new);
}
