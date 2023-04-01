/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 12:08:29 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 12:11:01 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*cmd_lstlast(t_cmd *lst)
{
	t_cmd	*ptr;

	if (!lst)
		return (NULL);
	ptr = lst;
	while (ptr->next != NULL)
		ptr = ptr->next;
	return (ptr);
}

void	cmd_lstadd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*ptr;

	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = cmd_lstlast(*lst);
	ptr->next = new;
	new->prev = ptr;
}

t_cmd	*cmd_lstnew(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		perror("Minishell: malloc()");
		return (NULL);
	}
	cmd->prev = NULL;
	cmd->wd = NULL;
	cmd->fd_infile = 0;
	cmd->infile_name = NULL;
	cmd->infile_mode = CLASSIC;
	cmd->fd_outfile = 1;
	cmd->outfile_name = NULL;
	cmd->outfile_mode = CLASSIC;
	cmd->good_path = NULL;
	cmd->position = 1;
	cmd->next = NULL;
	return (lexer);
}