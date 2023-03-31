/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:30:40 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/31 11:40:18 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	token_redir_or_pipe(char *input, int *index)
// {
// 	int		i;
// 	t_lexer	*buf;

// 	i = 0;
// 	while (input[*index + i] && (is_redir_or_pipe(input[*index + i]) || \
// 			input[*index + i] == ' '))
// 		i++;
// 	buf = lex_lstnew();
// 	if (!buf)
// 		return (FAILURE);
// 	buf->id = 1;
// 	buf->token = ft_strtrim(ft_substr(input, *index, i + 1), " ");
// 	if (!buf->token)
// 	{	
// 		free(buf);
// 		return (FAILURE);
// 	}
// 	lex_lstadd_back(&g_all.lexer, buf);
// 	*index += i;
// 	return (SUCCESS);
// }

// int	add_lst_for_quotes(char *input, int begin, int length, int id)
// {
// 	t_lexer	*buf;

// 	buf = lex_lstnew();
// 	if (!buf)
// 		return (FAILURE);
// 	buf->token = ft_substr(input, begin, length);
// 	if (!buf->token)
// 	{
// 		free(buf);
// 		return (FAILURE);
// 	}
// 	buf->id = id;
// 	lex_lstadd_back(&g_all.lexer, buf);
// 	return (SUCCESS);
// }

// int	token_simple_quote(char *input, int *index, int i, int id)
// {
// 	(*index)++;
// 	while (input[*index + (++i)])
// 	{
// 		if (input[*index] + i == 39)
// 		{
// 			if (i == 0 && input[*index + 1] != '\0')
// 			{
// 				(*index) += 1;
// 				return (token_unspecific(input, index));
// 			}
// 			else
// 				break ;
// 		}
// 	}
// 	if (!input[*index + i])
// 	{
// 		write_error("Minishell: ", "simple quotes are not even", "\n");
// 		return (FAILURE);
// 	}
// 	if (add_lst_for_quotes(input, *index, i, id) == FAILURE)
// 		return (FAILURE);
// 	*index += i + 1;
// 	return (SUCCESS);
// }

// int	token_double_quote(char *input, int *index, int i, int id)
// {
// 	(*index)++;
// 	while (input[*index + (++i)])
// 	{
// 		if (input[*index] == '$')
// 			id = VAR;
// 		else if (input[*index] + i == 34)
// 		{
// 			if (i == 0 && input[*index + 1] != '\0')
// 			{
// 				(*index) += 1;
// 				return (token_unspecific(input, index));
// 			}
// 			else
// 				break ;
// 		}
// 	}
// 	if (!input[*index + i])
// 	{
// 		write_error("Minishell: ", "double quotes are not even", "\n");
// 		return (FAILURE);
// 	}
// 	if (add_lst_for_quotes(input, *index, i, id) == FAILURE)
// 		return (FAILURE);
// 	*index += i + 1;
// 	return (SUCCESS);
// }