/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 14:28:15 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_all;

void	lexer(char *input)
{
	g_all.lexer = NULL;
	if (init_token_id(input) == FAILURE)
		return ;
	if (formate_token_id() == FAILURE) // sert a mettre un id en fonction du type de caracteres spe / fichier etc
		return ;
		
	t_list *tmp = g_all.lexer;
	while (tmp)
	{
		printf("TOKEN :%s\n", tmp->token);
		printf("TOKEN_ID :%d\n\n", tmp->id);
		tmp = tmp->next;
	}
	printf("Nb cmd = %d\n", g_all.nb_cmd);//***
	parser(-1);
}

int	init_token_id(char *input)
{
	int	i;
	int	result;

	i = 0;
	result = SUCCESS;
	while (input[i])
	{
		if (is_redir_or_pipe(input[i]))
			result = token_redir_or_pipe(input, &i);
		else if (input[i] == 39)
			result = token_simple_quote(input, &i);
		else if (input[i] == 34)
			result = token_double_quote(input, &i);
		else if (input[i] == '$')
			result = token_var(input, &i);
		else if (isnt_specific(input[i]) == SUCCESS)
			result = token_unspecific(input, &i);
		else
			i++;
		if (result == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	formate_token_id(void)
{
	t_list	*tmp;

	tmp = g_all.lexer;
	while (tmp)
	{
		if (tmp->id == 0)
			is_token_a_file(tmp);
		else if (tmp->id == 1)
		{
			if (formate_redir_or_pipe(tmp) == FAILURE)
				return (FAILURE);
		}
		// else if (tmp->id == 3)
		// 	replace_var(tmp);
		tmp = tmp->next;
	}
	return (SUCCESS);
}