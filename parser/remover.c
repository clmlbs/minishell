/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remover.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 21:08:01 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 21:17:10 by cleblais         ###   ########.fr       */
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

void	remove_token(t_lexer *lst)
{
	t_lexer	*buf;

	if (!lst->prev && !lst->next)
		free_one_lst_lexer(lst);
	else if (!lst->next)
	{
		lst->prev->next = NULL;
		free_one_lst_lexer(lst);
	}
	else if (!lst->prev)
	{
		buf = lst->next;
		buf->prev = NULL;
		free_one_lst_lexer(lst);
		g_all.lexer = buf;
	}
	else
	{
		buf = lst->prev;
		buf->next = lst->next;
		buf->next->prev = buf;
		free_one_lst_lexer(lst);
	}
}

void	remove_useless_token(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == SUPPR)
		{
			remove_token(buf);
			buf = g_all.lexer;
		}
		else
			buf = buf->next;
	}
}
