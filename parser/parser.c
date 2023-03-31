/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:15:19 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/31 13:52:46 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes_even(t_lexer *lst)
{
	int		nb_quotes;
	int		i;

	i = 0;
	nb_quotes = 0;
	while (lst->token[i])
	{
		if (lst->token[i] == lst->id)
			nb_quotes++;
		i++;
	}
	if (nb_quotes % 2 == 0)
		return (SUCCESS);
	else
	{
		printf("quotes = %d\n", nb_quotes);//****
		if (lst->id == SIMPLE_QUOTE)
			write_error("Minishell: ", "error: quotes `'' ", "are not even\n");
		else
			write_error("Minishell: ", "quotes `\"' ", "are not even\n");
		return (FAILURE);
	}
}

int	remove_quotes(t_lexer *lst)
{
	int		len;
	char	*new;

	len = ft_strlen(lst->token);
	new = ft_substr(lst->token, 1, len - 2);
	if (!new)
		return (FAILURE);
	free(lst->token);
	lst->token = new;
	return (SUCCESS);
}


int	are_quotes_even(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == SIMPLE_QUOTE || buf->id == DOUBLE_QUOTE)
		{
			if (check_quotes_even(buf) == FAILURE)
				return (FAILURE);
			if (remove_quotes(buf) == FAILURE)
				return (FAILURE);
		}
		buf = buf->next;
	}
	return (SUCCESS);
}


int	parser(void)
{
	if (are_quotes_even() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}