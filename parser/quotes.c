/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 14:12:11 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 21:16:54 by cleblais         ###   ########.fr       */
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

	len = ms_strlen(lst->token);
	new = ft_substr(lst->token, 1, len - 2);
	if (!new)
		return (FAILURE);
	free(lst->token);
	lst->token = new;
	if (len == 2 && lst->prev && lst->prev->id == WORD \
		&& lst->next && lst->next->id == WORD)
		lst->id = SUPPR;
	else
		lst->id = WORD;
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
