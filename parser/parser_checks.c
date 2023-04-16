/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 21:10:54 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 21:14:59 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_pipes(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == PIPE)
		{
			if (ms_strlen(buf->token) != 1)
			{
				error_token(buf, ms_strlen(buf->token), PIPE);
				return (FAILURE);
			}
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	check_redir(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if ((buf->id == REDIR_IN || buf->id == REDIR_OUT) && !buf->next)
		{
			write_error("Minishell: ", "syntax error near", \
			" unexpected token `newline'\n");
			return (FAILURE);
		}
		if (buf->id == REDIR_IN || buf->id == REDIR_OUT)
		{
			if (ms_strlen(buf->token) < 1 || ms_strlen(buf->token) > 2)
			{
				error_token(buf, ms_strlen(buf->token), REDIR);
				return (FAILURE);
			}
			change_id_redir(buf);
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	check_pipes_begin(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->prev == NULL && buf->id == PIPE)
		{
			write_error("Minishell: syntax error near unexpected ", \
			"token `|'", "\n");
			return (FAILURE);
		}
		buf = buf->next;
	}
	buf = lex_lstlast(g_all.lexer);
	if (buf->id == PIPE)
	{
		write_error("Minishell: syntax error near unexpected ", \
			"token `|'", "\n");
		return (FAILURE);
	}
	return (SUCCESS);
}
