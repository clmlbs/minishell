/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 14:28:46 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/07 16:08:32 by cleblais         ###   ########.fr       */
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
	char	*str_begin;
	char	*var_name;
	int		i;
	int		begin;
	char 	*var_value;
	char	*old_and_var_value;

	str_begin = NULL;
	i = 0;
	if (add_until_dollar(&str_begin, &i, *index, lexer) == FAILURE)
		return (FAILURE);
	i++;
	begin = i;
	if (!lexer->token[i] || is_space(lexer->token[i]) == TRUE)
		return(end_of_token(&str_begin, lexer, index));

	if (lexer->token[i] == '\"' && !(lexer->token[i + 1]))
		return(end_of_token_in_quotes(&str_begin, lexer, index, &i));
	go_until_end_of_var(lexer, &i);
	var_name = ft_substr(lexer->token, begin, i - begin);
	if (!var_name)
	{
		free(str_begin);
		return (FAILURE);
	}
	var_value = ptr_to_begin_of_var_value(var_name);
	printf("var_value:%s\n", var_value);//************
	free(var_name);
	if (!var_value)
	{
		free(str_begin);
		return (FAILURE);
	}
	old_and_var_value = ms_strjoin(str_begin, var_value);
	free(str_begin);
	free(var_value);
	if (!old_and_var_value)
		return (FAILURE);
	(*index) = ft_strlen(old_and_var_value);
	str_begin = ms_strjoin(old_and_var_value, \
		ft_substr(lexer->token, i, ft_strlen(lexer->token) - i));
	free(old_and_var_value);
	if (!str_begin)
		return (FAILURE);
	lexer->save = lexer->token;
	lexer->token = str_begin;
	return (SUCCESS);
}