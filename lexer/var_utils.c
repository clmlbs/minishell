/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:56:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 12:29:55 by cleblais         ###   ########.fr       */
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

int	add_until_dollar(char **str, int *i, int index, t_lexer *lexer)
{
	char	*new;

	while ((*i) < index)
	{
		new = ft_add_char_to_str(ft_strdup(*str), lexer->token[*i]);
		if (*str)
			free(*str);
		if (!new)
			return (FAILURE);
		*str = new;
		new = NULL;
		(*i)++;
	}
	return (SUCCESS);
}

int	end_of_dollar(char **str, t_lexer *lexer, int *index)
{
	char	*new;

	new = ft_add_char_to_str(ft_strdup(*str), '$');
	if (*str)
		free(*str);
	free(lexer->token);
	if (!new)
		return (FAILURE);
	lexer->token = new;
	new = NULL;
	(*index)++;
	return (SUCCESS);
}

int	end_of_dollar_in_quotes(char **str, t_lexer *lexer, int *index, int *i)
{	
	char	*new;

	new = ft_add_char_to_str(ft_strdup(*str), '$');
	if (*str)
		free(*str);
	if (!new)
		return (FAILURE);
	*str = new;
	new = NULL;
	new = ft_add_char_to_str(ft_strdup(*str), lexer->token[*i]);
	free(*str);
	free(lexer->token);
	if (!new)
		return (FAILURE);
	lexer->token = new;
	new = NULL;
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
	if (is_special_char_or_numeric(lexer->token[*i]) == TRUE && \
		lexer->token[*i] != '_')
		(*i)++;
	else
	{
		(*i)++;
		while (lexer->token[*i] && (ft_isalnum(lexer->token[*i]) == TRUE \
			|| lexer->token[*i] == '_'))
			(*i)++;
	}
}