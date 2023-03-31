/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/31 13:48:03 by cleblais         ###   ########.fr       */
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
		if (init_id(input[i]) == FAILURE)
			return ;
		i++;
	}
	if (tokenize_all_steps() == FAILURE)
		return ;
	return ;
}

int	tokenize_all_steps(void)
{
	if (tokenize_until_same(SIMPLE_QUOTE, NO) == FAILURE)
		return (FAILURE);
	if (tokenize_until_same(DOUBLE_QUOTE, NO) == FAILURE)
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