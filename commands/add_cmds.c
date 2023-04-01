/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:45:50 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 19:24:22 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_word(t_lexer *lexer, t_cmd *cmd)
{
	if (!cmd->wd)
		cmd->position = g_all.nb_cmd;
	if (update_wd(lexer->token, cmd) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	add_infile_name(t_lexer *lexer, t_cmd *cmd)
{
	cmd->infile_name = ft_strdup(lexer->token);
	if (!cmd->infile_name)
		return (ft_perror("Minishell: malloc()"));
	cmd->infile_mode = READ;
	return (SUCCESS);
}

int	add_outfile_name(t_lexer *lexer, t_cmd *cmd)
{
	printf("ici\n");//******
	cmd->outfile_name = ft_strdup(lexer->token);
	if (!cmd->outfile_name)
		return (ft_perror("Minishell: malloc()"));
	if (lexer->prev->id == SIMPLE_REDIR_OUT)
		cmd->outfile_mode = REPLACE;
	else if (lexer->prev->id == DOUBLE_REDIR_OUT)
		cmd->outfile_mode = APPEND;
	else
	{
		write_error("Minishell: ", "syntax error", "\n");
		return (FAILURE);
	}
	return (SUCCESS);
}