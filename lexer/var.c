/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 14:28:46 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/14 19:23:57 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
				else // je viens de rajouter un else 
					i++;
			}
			i = 0;
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	update_token(t_lexer *lexer, int *index)
{
	t_update_token	t;

	init_t_updated_token(&t);
	if (add_until_dollar(&(t.str_begin), &(t.i), *index, lexer) == FAILURE)
		return (FAILURE);
	(t.i)++;
	t.begin = t.i;
	if (!lexer->token[t.i] || is_space(lexer->token[t.i]) == TRUE)
		return(end_of_token(&(t.str_begin), lexer, index));
	if (lexer->token[t.i] == '\"' && !(lexer->token[t.i + 1]))
		return(end_of_token_in_quotes(&(t.str_begin), lexer, index, &(t.i)));
	go_until_end_of_var(lexer, &(t.i));
	if (create_token_updated(lexer, &t, index) == FAILURE)
		return (FAILURE);
	lexer->save = lexer->token;
	lexer->token = t.token_updated;
	return (SUCCESS);
}

int	create_var_name(t_lexer *lexer, t_update_token *t)
{
	t->var_name = ft_substr(lexer->token, t->begin, t->i - t->begin);
	if (!(t->var_name))
	{
		free(t->str_begin);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	create_token_updated(t_lexer *lexer, t_update_token *t, int *index)
{
	if (create_var_name(lexer, t) == FAILURE)
		return (FAILURE);
	t->var_value = create_var_value(t->var_name);
	free(t->var_name);
	if (!(t->var_value))
	{
		free(t->str_begin);
		return (FAILURE);
	}
	t->begin_and_value = ms_strjoin(t->str_begin, t->var_value);
	free(t->str_begin);
	free(t->var_value);
	if (!(t->begin_and_value))
		return (FAILURE);
	(*index) = ms_strlen(t->begin_and_value);
	t->token_updated = ms_strjoin(t->begin_and_value, \
		ft_substr(lexer->token, t->i, ms_strlen(lexer->token) - t->i));
	free(t->begin_and_value);
	if (!(t->token_updated))
		return (FAILURE);
	return (SUCCESS);
}