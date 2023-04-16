/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:53:23 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 20:15:30 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_t_updated_token(t_update_token *t)
{
	t->i = 0;
	t->begin = 0;
	t->str_begin = NULL;
	t->var_name = NULL;
	t->var_value = NULL;
	t->begin_and_value = NULL;
	t->token_updated = NULL;
}

char	*var_is_exit_status(void)
{
	char	*new;

	new = ft_itoa(g_all.status);
	if (!new)
	{
		perror("Minishell");
		return (NULL);
	}
	return (new);
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
