/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 14:28:46 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 12:28:33 by cleblais         ###   ########.fr       */
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
				else
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
	if (add_until_dollar(&(t.str_begin), &(t.i), *index, lexer) == FAILURE) // PLUS DE LEAKS
		return (FAILURE);
	(t.i)++;
	t.begin = t.i;
	if (!lexer->token[t.i] || is_space(lexer->token[t.i]) == TRUE) // si y'a juste $ PLUS DE LEAK
		return(end_of_dollar(&(t.str_begin), lexer, index));
	if (lexer->token[t.i] == '\"' && !(lexer->token[t.i + 1])) // "$" theoriquement plus de leak
		return(end_of_dollar_in_quotes(&(t.str_begin), lexer, index, &(t.i)));
	go_until_end_of_var(lexer, &(t.i));
	if (create_token_updated(lexer, &t, index) == FAILURE)
		return (FAILURE);
	if (lexer->save == NULL)
		lexer->save = lexer->token;
	else if (lexer->token)
		free(lexer->token);
	lexer->token = t.token_updated;
	t.token_updated = NULL;
	return (SUCCESS);
}

int	create_var_name(t_lexer *lexer, t_update_token *t)
{
	t->var_name = ft_substr(lexer->token, t->begin, t->i - t->begin);
	if (!(t->var_name))
	{
		free(t->str_begin);
		t->str_begin = NULL;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	free_update_token(t_update_token *t, int return_value)
{
	if (t->str_begin)
		free(t->str_begin);
	if (t->var_name)
		free(t->var_name);
	if (t->var_value)
		free(t->var_value);
	if (t->begin_and_value)
		free(t->begin_and_value);
	return (return_value);
}


int	create_token_updated(t_lexer *lexer, t_update_token *t, int *index)
{
	char	*rest;

	if (create_var_name(lexer, t) == FAILURE)
		return (free_update_token(t, FAILURE));
	//printf("====== NEW =======:%p\n", t->var_name);//******
	t->var_value = create_var_value(t->var_name);
	if (!(t->var_value))
		return (free_update_token(t, FAILURE));
	t->begin_and_value = ms_strjoin(t->str_begin, t->var_value);
	// ca leak avant ca 
	if (!(t->begin_and_value))
		return (free_update_token(t, FAILURE));
	(*index) = ms_strlen(t->begin_and_value);
	rest = ft_substr(lexer->token, t->i, ms_strlen(lexer->token) - t->i);
	if (!rest)
		return (free_update_token(t, FAILURE));
	t->token_updated = ms_strjoin(t->begin_and_value, rest);
	free(rest);
	if (!(t->token_updated))
		return (free_update_token(t, FAILURE));
	return (free_update_token(t, SUCCESS));
}