/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:25:52 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/03 11:48:52 by cleblais         ###   ########.fr       */
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

void	free_t_lexer(t_lexer *lst)
{
	if (!lst)
		return ;
	if (lst->token)
		free(lst->token);
	free(lst);
	lst = NULL;//mettre ou pas ca ? 
}

void	free_all_lexer(void)
{
	t_lexer	*buf;
	t_lexer	*lx;

	lx = g_all.lexer;
	if (lx)
	{
		while (lx)
		{
			buf = lx->next;
			if (lx->token)
				free(lx->token);
			free(lx);
			lx = buf;
		}
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

void	free_all_cmd(void)
{
	t_cmd	*buf;
	t_cmd	*cmd;

	cmd = g_all.cmd;
	if (cmd)
	{
		while (cmd)
		{
			buf = cmd->next;
			free_t_cmd(cmd);
			cmd = buf;
		}
	}
}

void	free_all_lexer_and_cmd(void)
{
	free_all_lexer();
	free_all_cmd();
	g_all.lexer = NULL;
	g_all.cmd = cmd_lstnew();
	if (!g_all.cmd)
		exit(1);// bon status de sortie ? 
	g_all.nb_cmd = 0;
}