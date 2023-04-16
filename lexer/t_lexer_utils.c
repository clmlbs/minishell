/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_lexer_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:43:04 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 11:09:53 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_next_char_to_current(t_lexer *current)
{
	t_lexer	*buf_to_suppr;

	buf_to_suppr = current->next;	
	if (add_char_to_str(current, current->next) == FAILURE)
		return (FAILURE);
	if (current->next->next)
	{
		current->next = current->next->next;
		current->next->prev = current;
	}
	else
		current->next = NULL;
	free_one_lst_lexer(buf_to_suppr);
	return (SUCCESS);
}

int	add_char_to_str(t_lexer *str_dest, t_lexer *char_src)
{
	char	unique_char[2];
	char	*str_buf;

	unique_char[0] = char_src->c;
	unique_char[1] = '\0';
	str_buf = ms_strjoin(str_dest->token, unique_char);
	if (!str_buf)
		return (FAILURE);
	free(str_dest->token);
	str_dest->token = str_buf;
	return (SUCCESS);
}

char	*ft_add_char_to_str(char *str, char c)
{
	char	unique_char[2];
	char	*new;

	unique_char[0] = c;
	unique_char[1] = '\0';
	new = ms_strjoin(str, unique_char);
	if (str)
		free(str);
	if (!new)
		return (NULL);
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