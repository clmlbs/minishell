/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:25:52 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 19:27:37 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_all_lexer(void)
{
	t_lexer	*buf;
	t_lexer	*lx;

	lx = g_all.lexer;
	while (lx)
	{
		buf = lx->next;
		free_one_lst_lexer(lx);
		lx = buf;
	}
}

void	free_everything(void)
{
	close(g_all.fd_stdin);
	close(g_all.fd_stdout);
	close(g_all.herit[0]);
	close(g_all.herit[1]);
	if (g_all.all_path)
		free_tab_strs(g_all.all_path);
	if (g_all.env)
		free_tab_strs(g_all.env);
	if (g_all.tilde)
		free(g_all.tilde);
	if (g_all.pid)
		free(g_all.pid);
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
	free(cmd);
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
		exit(FAILURE);
	g_all.nb_cmd = 0;
}
