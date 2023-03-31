/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:25:52 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/31 15:52:46 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tab_strs(char **str)
{
	int	i;

	i = 0;
	if (str)
	{
		while (str[i])
		{
			free (str[i]);
			i++;
		}
		free(str[i]);
		free (str);
	}
}

void	free_t_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->wd)
		free_tab_strs(cmd->wd);
	if (cmd->infile_name)
		free(cmd->infile_name);
	if (cmd->outfile_name)
		free(cmd->outfile_name);
	if (cmd->good_path)
		free(cmd->good_path);
	free(cmd); // ok ca ? 
}

void	free_t_lexer(t_lexer *lst)
{
	if (!lst)
		return ;
	if (lst->token)
		free(lst->token);
	free(lst);
	lst = NULL;//mettre ou pas ca ? 
}