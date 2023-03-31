/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 10:43:22 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/31 09:21:53 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_token_a_file(t_lexer *lst)
{
	if (lst->prev)
	{
		if (lst->prev->id == SIMPLE_REDIR_IN || \
			lst->prev->id == SIMPLE_REDIR_OUT || \
			lst->prev->id == DOUBLE_REDIR_OUT)
			lst->id = FILE_NAME;
		if (lst->prev->id == DOUBLE_REDIR_IN)
			lst->id = KEY_WORD_HERE_DOC;
	}
}

void	token_is_unique(t_lexer *lst)
{
	if (lst->token[0] == '|')
	{
		lst->id = PIPE;
		g_all.nb_cmd++;
	}
	else if (lst->token[0] == '<')
		lst->id = SIMPLE_REDIR_IN;
	else if (lst->token[0] == '>')
		lst->id = SIMPLE_REDIR_OUT;
	else
		write_error("Minishell:", "tokenisation error", "\n");
}

int	token_is_double(t_lexer *lst)
{
	if (lst->token[0] != lst->token[1])
	{
		write_error("Minishell:", " syntax error near unexpected", \
			" token `newline'\n");
		return (FAILURE);
	}
	else if (lst->token[0] == '|')
	{
		write_error("Minishell:", " || are not implemented in this", \
			" Minishell\n");
		return (FAILURE);
	}
	else if (lst->token[0] == '<')
		lst->id = DOUBLE_REDIR_IN;
	else if (lst->token[0] == '>')
		lst->id = DOUBLE_REDIR_OUT;
	else
	{
		write_error("Minishell:", "tokenisation error", "\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	formate_redir_or_pipe(t_lexer *lst)
{
	int	len;

	len = ft_strlen(lst->token);
	if (len == 0)
	{
		write_error("Minishell:", "tokenisation error", "\n");
		return (FAILURE);
	}
	if (len == 1)
		token_is_unique(lst);
	else if (len == 2)
	{
		if (token_is_double(lst) == FAILURE)
			return (FAILURE);
	}
	else if (len > 2)
	{
		error_token(lst, len);
		return (FAILURE);
	}
	return (SUCCESS);
}
