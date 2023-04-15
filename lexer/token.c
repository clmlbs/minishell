/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:45:33 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/15 12:33:31 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_id(char c)
{
	int	result;

	result = SUCCESS;
	if (c == ' ' || c == 9 || c == 11)
		result = assign_id_to_char(c, WHITESPACE);
	else if (c == '\n')
		result = assign_id_to_char(c, BACKSLASH_N);
	else if (c == '$')
		result = assign_id_to_char(c, VAR);
	else if (c == '|')
		result = assign_id_to_char(c, PIPE);
	else if (c == '<')
		result = assign_id_to_char(c, REDIR_IN);
	else if (c == '>')
		result = assign_id_to_char(c, REDIR_OUT);
	else if (c == '\'')
		result = assign_id_to_char(c, SIMPLE_QUOTE);
	else if (c == '\"')
		result = assign_id_to_char(c, DOUBLE_QUOTE);
	else if (is_special_char(c) == TRUE && c != '_')
		result = assign_id_to_char(c, CHAR_SPE);
	else if (ft_isalnum(c) == TRUE || c == '_')
		result = assign_id_to_char(c, WORD);
	else
		return (FAILURE);
	return (result);
}

int	assign_id_to_char(char c, int id)
{
	t_lexer	*new;
	
	new = lex_lstnew();
	if (!new)
		return (FAILURE);
	new->c = c;
	new->id = id;
	if (g_all.lexer->c == 0)
	{
		free_one_lst_lexer(g_all.lexer);
		g_all.lexer = new;
	}
	else
		lex_lstadd_back(&g_all.lexer, new);
	return (SUCCESS);
}

int	tokenize_words(int id_target, int id_compare)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == id_target)
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