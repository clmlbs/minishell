/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserbad.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 13:11:29 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 15:07:34 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parser(int i)
{
	t_cmd	cmd;
	t_lexer	*tmp;

	tmp = g_all.lexer;
	while (tmp && ++i < g_all.nb_cmd)
	{
		if (init_t_cmd(&cmd) == FAILURE)
			return ;
		while (tmp != NULL && tmp->id != PIPE)
		{
			if (fill_cmd(&cmd, tmp) == FAILURE)
				return ;
			tmp = tmp->next;
		}
		printf_strs(cmd.wd);//********
		//execute_cmd(&cmd);
		free_t_cmd(&cmd);
		if (tmp && tmp->id == PIPE)
			tmp = tmp->next;
	}
}

int	fill_cmd(t_cmd *cmd, t_lexer *lst)
{
	if (lst->id == WORD || lst->id == VAR)
	{
		if (update_wd(cmd, lst) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}