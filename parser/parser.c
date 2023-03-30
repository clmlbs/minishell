/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 13:11:29 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 10:47:34 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parser(int i)
{
	t_cmd	cmd;
	t_list	*tmp;

	tmp = g_ctx.lexer;
	while (tmp && ++i < g_ctx.nb_cmd)
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

int	update_wd(t_cmd *cmd, t_list *lst)
{
	char	**new;
	int		index;

	index = tab_strlen(cmd->wd);
	new = (char **)malloc(sizeof(char *) * (index + 2));
	if (!new)
		return (ft_perror("Minishell: malloc()", FAILURE));
	if (copy_tab_of_strs(cmd->wd, new) == FAILURE)
		return (FAILURE);
	new[index] = ft_strdup(lst->token);
	if (!new[index])
	{
		free_tab_strs(new);
		return (ft_perror("Minishell: ft_strdup()"));
	}
	new[index + 1] = NULL;
	free_tab_strs(cmd->wd);
	cmd->wd = new;
	return (SUCCESS);
}

int	fill_cmd(t_cmd *cmd, t_list *lst)
{
	if (lst->id == WORD || lst->id == VAR)
	{
		if (update_wd(cmd, lst) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}