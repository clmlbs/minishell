/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/03 13:36:51 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_all;

int	lexer(char *input)
{
	int	i;

	i = 0;
	g_all.lexer = lex_lstnew();
	if (!g_all.lexer)
		return (FAILURE);
	//printf("input:%s\n", input);//*****
	while (input[i])
	{
		if (init_id(input[i]) == FAILURE)
		{
			write_error("Minishell: ", "syntax error", "\n");
			return (FAILURE);
		}
		i++;
	}
	if (tokenize_all_steps() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	tokenize_all_steps(void)
{
	if (tokenize_quotes(NO, 0) == FAILURE)
		return (FAILURE);
	if (tokenize_words(VAR, WORD) == FAILURE)
		return (FAILURE);
	if (tokenize_words(WORD, WORD) == FAILURE)
		return (FAILURE);
	if (tokenize_words(PIPE, PIPE) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_IN, REDIR_IN) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_OUT, REDIR_OUT) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}