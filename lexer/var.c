/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 14:28:46 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/07 16:20:33 by cleblais         ###   ########.fr       */
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

int	replace_var(void)
{
	t_lexer	*buf;
	int		i;

	i = 0;
	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == VAR)
		{
			while (buf->token[i])
			{
				if (buf->token[i] == '$')
				{
					if (update_token(buf, &i) == FAILURE)
						return (FAILURE);
					if (buf->token[0] == '\"')
						buf->id = DOUBLE_QUOTE;
					else
						buf->id = WORD;
				}
				i++;
			}
			i = 0;
		}
		buf = buf->next;
	}
	return (SUCCESS);
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

int	update_token(t_lexer *lexer, int *index)
{
	t_update_token	t;
	
	t.str_begin = NULL;
	t.i = 0;
	if (add_until_dollar(&(t.str_begin), &(t.i), *index, lexer) == FAILURE)
		return (FAILURE);
	(t.i)++;
	t.begin = t.i;
	if (!lexer->token[t.i] || is_space(lexer->token[t.i]) == TRUE)
		return(end_of_token(&(t.str_begin), lexer, index));

	if (lexer->token[t.i] == '\"' && !(lexer->token[t.i + 1]))
		return(end_of_token_in_quotes(&(t.str_begin), lexer, index, &(t.i)));
	go_until_end_of_var(lexer, &(t.i));
	t.var_name = ft_substr(lexer->token, t.begin, t.i - t.begin);
	if (!(t.var_name))
	{
		free(t.str_begin);
		return (FAILURE);
	}
	t.var_value = ptr_to_begin_of_var_value(t.var_name);
	free(t.var_name);
	if (!(t.var_value))
	{
		free(t.str_begin);
		return (FAILURE);
	}
	t.begin_and_value = ms_strjoin(t.str_begin, t.var_value);
	free(t.str_begin);
	free(t.var_value);
	if (!(t.begin_and_value))
		return (FAILURE);
	(*index) = ft_strlen(t.begin_and_value);
	t.token_updated = ms_strjoin(t.begin_and_value, \
		ft_substr(lexer->token, t.i, ft_strlen(lexer->token) - t.i));
	free(t.begin_and_value);
	if (!(t.token_updated))
		return (FAILURE);
	lexer->save = lexer->token;
	lexer->token = t.token_updated;
	return (SUCCESS);
}