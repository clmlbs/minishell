/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 11:47:38 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 18:05:24 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int	add_token_to_cmd(t_lexer *lexer, t_cmd *cmd)
{
	int	result;

	result = SUCCESS;
	if (lexer->id == KEY_WORD_HERE_DOC)
		result = add_key_word_here_doc(lexer);
	if (lexer->id == WORD && result == SUCCESS)
		result = add_word(lexer, cmd);
	else if (lexer->id == INFILE_NAME)
		result = add_infile_name(lexer, cmd);
	// else if (lexer->id == OUTFILE_NAME)
	// 	result = add_outile_name(lexer, cmd);
	// else if (lexer->id == SIMPLE_REDIR_IN)
	// 	result = add_simple_redir_in(lexer, cmd);
	// else if (lexer->id == DOUBLE_REDIR_IN)
	// 	result = add_double_redir_in(lexer, cmd);
	// else if (lexer->id == SIMPLE_REDIR_OUT)
	// 	result = add_simple_redir_out(lexer, cmd);
	// else if (lexer->id == DOUBLE_REDIR_OUT)
	// 	result = add_double_redir_out(lexer, cmd);
	// else
	// 	return (FAILURE);
	return (result);
}

int	fill_t_cmd(void)
{
	t_lexer	*lexer;
	t_cmd	*cmd;
	
	lexer = g_all.lexer;
	cmd = g_all.cmd;
	while (lexer)
	{
		if (lexer->id == PIPE)
		{
			g_all.nb_cmd++;
			cmd = cmd_lstnew();
			if (!cmd)
				return (FAILURE);
			cmd_lstadd_back(&g_all.cmd, cmd);
		}
		if (add_token_to_cmd(lexer,cmd) == FAILURE)
			return (FAILURE);
		lexer = lexer->next;
	}	
	return (SUCCESS);
}