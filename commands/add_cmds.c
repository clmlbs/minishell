/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:45:50 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 15:10:35 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	update_wd(t_lexer *lexer, t_cmd *cmd)
{
	char	**new;
	int		index;

	index = tab_strlen(cmd->wd);
	new = (char **)malloc(sizeof(char *) * (index + 2));
	if (!new)
		return (ft_perror("Minishell: malloc()"));
	if (copy_tab_of_strs(cmd->wd, new) == FAILURE)
	{
		free(new);
		return (FAILURE);
	}
	new[index] = ft_strdup(lexer->token);
	if (!new[index])
	{
		free_tab_strs(new);
		return (FAILURE);
	}
	new[index + 1] = NULL;
	free_tab_strs(cmd->wd);
	cmd->wd = new;
	return (SUCCESS);
}

int	add_word(t_lexer *lexer, t_cmd *cmd)
{
	if (!cmd->wd)
		cmd->position = g_all.nb_cmd;
	if (update_wd(lexer, cmd) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}