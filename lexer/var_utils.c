/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:56:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/14 19:19:01 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_id_var(void)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == DOUBLE_QUOTE && ft_strchr(buf->token, '$'))
			buf->id = VAR;
		buf = buf->next;
	}
}

int	add_until_dollar(char **new, int *i, int index, t_lexer *lexer)
{
	while ((*i) < index)
	{
		(*new) = ft_add_char_to_str(ft_strdup(*new), lexer->token[*i]);
		if (!(*new))
			return (FAILURE);
		(*i)++;
	}
	return (SUCCESS);
}

int	end_of_token(char **new, t_lexer *lexer, int *index)
{
	(*new) = ft_add_char_to_str(ft_strdup(*new), '$');
	if (!(*new))
		return (FAILURE);
	free(lexer->token);
	lexer->token = (*new);
	(*index)++;
	return (SUCCESS);
}

int	end_of_token_in_quotes(char **new, t_lexer *lexer, int *index, int *i)
{	
	(*new) = ft_add_char_to_str(ft_strdup(*new), '$');
	if (!(*new))
		return (FAILURE);
	(*new) = ft_add_char_to_str(ft_strdup(*new), lexer->token[*i]);
	if (!(*new))
		return (FAILURE);
	free(lexer->token);
	lexer->token = (*new);
	(*index)++;
	return (SUCCESS);
}

void	go_until_end_of_var(t_lexer *lexer, int *i)
{
	if (lexer->token[*i] == '?')
	{
		(*i)++;
		return ;
	}
	if (is_special_char_or_numeric(lexer->token[*i]) == TRUE)
		(*i)++;
	else
	{
		(*i)++;
		while (lexer->token[*i] && (ft_isalnum(lexer->token[*i]) == TRUE \
			|| lexer->token[*i] == '_'))
			(*i)++;
	}
}