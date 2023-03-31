/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/31 11:42:08 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_all;

void	lexer(char *input)
{
	int	i;

	i = 0;
	g_all.lexer = lex_lstnew();
	if (!g_all.lexer)
		return ;
	printf("input:%s\n", input);//***
	while (input[i])
	{
		if (init_tokenize(input[i]) == FAILURE)
			return ;
		i++;
	}
	if (tokenize_all_steps() == FAILURE)
		return ;
	print_t_lexer();//********
	return ;
}

int	tokenize_all_steps(void)
{
	if (tokenize_until_same(SIMPLE_QUOTE) == FAILURE)
		return (FAILURE);
	if (tokenize_until_same(DOUBLE_QUOTE) == FAILURE)
		return (FAILURE);
	if (tokenize_words(VAR, WORD) == FAILURE)
		return (FAILURE);
	if (tokenize_words(PIPE, PIPE) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_IN, REDIR_IN) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_OUT, REDIR_OUT) == FAILURE)
		return (FAILURE);
	if (tokenize_words(WORD, WORD) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	tokenize_until_same(int id)
{
	t_lexer *buf;
	int		even;

	even = NO;
	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == id)
		{
			if (buf->token == NULL)
			{
				if (add_char_to_str(buf, buf) == FAILURE)
					return (FAILURE);
			}
			while (even == NO && buf->next)
			{
				if (buf->next->id == id)
					even = YES;
				if (add_next_to_current(buf) == FAILURE)
					return(FAILURE);
			}
			even = NO;
		}
		buf = buf->next;
	}
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
				if (add_next_to_current(buf) == FAILURE)
					return (FAILURE);
			}
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	add_next_to_current(t_lexer *current)
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
	free_t_lexer(buf_to_suppr);
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

int	init_tokenize(char c)
{
	int	result;

	result = SUCCESS;
	if (c == ' ')
		result = tokenize_char(c, WHITESPACE);
	else if (c == '\n')
		result = tokenize_char(c, BACKSLASH_N);
	else if (c == '$')
		result = tokenize_char(c, VAR);
	else if (c == '|')
		result = tokenize_char(c, PIPE);
	else if (c == '<')
		result = tokenize_char(c, REDIR_IN);
	else if (c == '>')
		result = tokenize_char(c, REDIR_OUT);
	else if (c == '\'')
		result = tokenize_char(c, SIMPLE_QUOTE);
	else if (c == '\"')
		result = tokenize_char(c, DOUBLE_QUOTE);
	else if (c >= 33 && c <= 126)
		result = tokenize_char(c, WORD);
	else
		return (FAILURE);
	return (result);
}

int	tokenize_char(char c, int id)
{
	t_lexer	*new;
	
	new = lex_lstnew();
	if (!new)
		return (FAILURE);
	new->c = c;
	new->id = id;
	if (g_all.lexer->c == 0)
	{
		free_t_lexer(g_all.lexer);
		g_all.lexer = new;
	}
	else
		lex_lstadd_back(&g_all.lexer, new);
	return (SUCCESS);
}




// 	if (init_token_id(input) == FAILURE)
// 		return ;
// 	if (formate_token_id() == FAILURE) // sert a mettre un id en fonction du type de caracteres spe / fichier etc
// 		return ;
		
// 	t_lexer *tmp = g_all.lexer;
// 	while (tmp)
// 	{
// 		printf("TOKEN :%s\n", tmp->token);
// 		printf("TOKEN_ID :%d\n\n", tmp->id);
// 		tmp = tmp->next;
// 	}
// 	printf("Nb cmd = %d\n", g_all.nb_cmd);//***
// 	parser(-1);
// }

// int	init_token_id(char *input)
// {
// 	int	i;
// 	int	result;

// 	i = 0;
// 	result = SUCCESS;
// 	while (input[i])
// 	{
// 		if (is_redir_or_pipe(input[i]))
// 			result = token_redir_or_pipe(input, &i);
// 		else if (input[i] == 39)
// 			result = token_simple_quote(input, &i, -1, WORD);
// 		else if (input[i] == 34)
// 			result = token_double_quote(input, &i, -1, WORD);
// 		else if (input[i] == '$')
// 			result = token_var(input, &i);
// 		else if (isnt_specific(input[i]) == SUCCESS)
// 			result = token_unspecific(input, &i);
// 		else
// 			i++;
// 		if (result == FAILURE)
// 			return (FAILURE);
// 	}
// 	return (SUCCESS);
// }

// int	formate_token_id(void)
// {
// 	t_lexer	*tmp;

// 	tmp = g_all.lexer;
// 	while (tmp)
// 	{
// 		if (tmp->id == 0)
// 			is_token_a_file(tmp);
// 		else if (tmp->id == 1)
// 		{
// 			if (formate_redir_or_pipe(tmp) == FAILURE)
// 				return (FAILURE);
// 		}
// 		// else if (tmp->id == 3)
// 		// 	replace_var(tmp);
// 		tmp = tmp->next;
// 	}
// 	return (SUCCESS);
// }