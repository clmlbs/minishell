/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 11:17:29 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/14 14:14:25 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		perror_void("Minishell: malloc()");
		return (NULL);
	}
	lexer->prev = NULL;
	lexer->c = 0;
	lexer->token = NULL;
	lexer->save = NULL;
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
		ft_putstr_fd("id: ", 2);
		ft_putnbr_fd(buf->id, 2);
		ft_putstr_fd("		", 2);
		ft_putchar_fd(buf->c, 2);
		ft_putstr_fd("	", 2);
		ft_putstr_fd(buf->token, 2);
		if (buf->save)
		{
			ft_putstr_fd("	", 2);
			ft_putstr_fd(buf->save, 2);
		}
		ft_putstr_fd("\n", 2);
		buf = buf->next;
	}
	ft_putstr_fd("\n", 2);
}