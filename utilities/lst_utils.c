/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 11:17:29 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 14:28:39 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ms_lstclear(t_list **lst)
{
	t_list	*buf;

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

void	ms_lstdelone(t_list *lst)
{
	if (!lst)
		return ;
	// if (lst->wd)
	// 	free_tab_strs(lst->wd);
	if (lst->token)
		free(lst->token);
	// if (lst->name_env)
	// 	free(lst->name_env);
	// if (lst->path_env)
	// 	free(lst->path_env);
	free(lst);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*ptr;

	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = ft_lstlast(*lst);
	ptr->next = new;
	new->prev = ptr;
}

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*ptr;

	if (!lst)
		return (NULL);
	ptr = lst;
	while (ptr->next != NULL)
		ptr = ptr->next;
	return (ptr);
}

t_list	*new_lexer(void)
{
	t_list	*lexer;

	lexer = (t_list *)malloc(sizeof(t_list));
	if (!lexer)
	{
		perror("Minishell: malloc()");
		return (NULL);
	}
	lexer->prev = NULL;
	lexer->token = NULL;
	lexer->id = WORD;
	lexer->next = NULL;
	return (lexer);
}