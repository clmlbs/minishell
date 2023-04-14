/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 10:47:51 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/14 15:24:51 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_spaces(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == WHITESPACE || buf->id == BACKSLASH_N)
		{
			remove_token(buf);
			buf = g_all.lexer;
		}
		else
			buf = buf->next;
	}
}

int	add_next_str_to_current(t_lexer *current)
{
	char	*new;

	new = ms_strjoin(current->token, current->next->token);
	if (!new)
		return (FAILURE);
	free(current->token);
	current->token = new;
	remove_token(current->next);
	return (SUCCESS);
}

int	parse_same_id(int id_target)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == id_target && buf->next && buf->next->id == id_target)
		{
			if (add_next_str_to_current(buf) == FAILURE)
				return (FAILURE);
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

void	change_id_redir(t_lexer *lst)
{
	if (lst->id == REDIR_IN && ms_strlen(lst->token) == 1)
		lst->id = SIMPLE_REDIR_IN;
	else if (lst->id == REDIR_IN && ms_strlen(lst->token) == 2)
		lst->id = DOUBLE_REDIR_IN;
	else if (lst->id == REDIR_OUT && ms_strlen(lst->token) == 1)
		lst->id = SIMPLE_REDIR_OUT;
	else
		lst->id = DOUBLE_REDIR_OUT;
}