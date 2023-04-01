/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 11:17:29 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 12:07:11 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ms_lstclear(t_lexer **lst)
{
	t_lexer	*buf;

	if (lst)
	{
		while (*lst)
		{
			buf = (*lst)->next;
			ms_lstdelone((*lst));
			*lst = buf;
		}
	}
}

void	ms_lstdelone(t_lexer *lst)
{
	if (!lst)
		return ;
	if (lst->token)
		free(lst->token);
	free(lst);
}

void	lex_lstadd_back(t_lexer **lst, t_lexer *new)
{
	t_lexer	*ptr;

	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = lex_lstlast(*lst);
	ptr->next = new;
	new->prev = ptr;
}

t_lexer	*lex_lstlast(t_lexer *lst)
{
	t_lexer	*ptr;

	if (!lst)
		return (NULL);
	ptr = lst;
	while (ptr->next != NULL)
		ptr = ptr->next;
	return (ptr);
}

t_lexer	*lex_lstnew(void)
{
	t_lexer	*lexer;

	lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if (!lexer)
	{
		perror("Minishell: malloc()");
		return (NULL);
	}
	lexer->prev = NULL;
	lexer->c = 0;
	lexer->token = NULL;
	lexer->id = WORD;
	lexer->next = NULL;
	return (lexer);
}

void	print_t_lexer(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		printf("id:%d	%c	%s\n", buf->id, buf->c, buf->token);
		buf = buf->next;
	}
}