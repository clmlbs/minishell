/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:45:33 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 20:15:55 by cleblais         ###   ########.fr       */
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

int	charcat(t_lexer *buf, int id_compare)
{
	if (buf->token == NULL)
	{
		if (add_char_to_str(buf, buf) == FAILURE)
			return (FAILURE);
	}
	while (buf->next && (buf->next->id == id_compare))
	{
		if (buf->next->token)
		{
			if (add_next_str_to_current(buf) == FAILURE)
				return (FAILURE);
		}
		else if (add_next_char_to_current(buf) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	tokenize_words(int id_target, int id_compare)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == id_target)
		{
			if (charcat(buf, id_compare) == FAILURE)
				return (FAILURE);
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	tokenize_quotes(int even, int id)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == SIMPLE_QUOTE || buf->id == DOUBLE_QUOTE)
		{
			id = buf->id;
			if (buf->token == NULL)
			{
				if (add_char_to_str(buf, buf) == FAILURE)
					return (FAILURE);
			}
			while (even == NO && buf->next)
			{
				if (buf->next->id == id)
					even = YES;
				if (add_next_char_to_current(buf) == FAILURE)
					return (FAILURE);
			}
			even = NO;
		}
		buf = buf->next;
	}
	return (SUCCESS);
}
