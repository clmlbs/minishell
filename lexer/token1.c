/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:30:40 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 14:52:54 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	token_redir_or_pipe(char *input, int *index)
{
	int		i;
	t_list	*buf;

	i = 0;
	while (input[*index + i] && (is_redir_or_pipe(input[*index + i]) || \
			input[*index + i] == ' '))
		i++;
	buf = new_lexer();
	if (!buf)
		return (FAILURE);
	buf->id = 1;
	buf->token = ft_strtrim(ft_substr(input, index, i + 1), " ");
	if (!buf->token)
	{	
		free(buf);
		return (FAILURE);
	}
	ft_lstadd_back(&g_all.lexer, buf);
	*index += i;
	return (SUCCESS);
}

int	token_simple_quote(char *input, int *index)
{
	int		i;
	t_list	*buf;

	i = 0;
	while (input[*index + i + 1])
	{
		if (input[*index] + i + 1 == 39)
			break ;
			i++;
	}
	if (!input[*index + i + 1])
		write_error("Minishell: ", "simple quotes are not even", "\n");
	if (!input[*index + i + 1])
		return (FAILURE);
	buf = new_lexer();
	if (!buf)
		return (FAILURE);
	buf->token = ft_substr(input, index + 1, i);
	if (!buf->token)
		free(buf);
	if(!buf->token)
		return (FAILURE);
	ft_lstadd_back(&g_all.lexer, buf);
	*index += i + 2;
	return (SUCCESS);
}