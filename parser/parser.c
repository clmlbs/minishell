/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:15:19 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/31 16:55:10 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_pipes(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == PIPE)
		{
			if (ft_strlen(buf->token) != 1)
			{
				error_token(buf, ft_strlen(buf->token));
				return (FAILURE);
			}
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	check_redir(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == REDIR_IN || buf->id == REDIR_OUT)
		{
			if (ft_strlen(buf->token) < 1 || ft_strlen(buf->token) > 2)
			{
				error_token(buf, ft_strlen(buf->token));
				return (FAILURE);
			}
			if (buf->id == REDIR_IN && ft_strlen(buf->token) == 1)
				buf->id = SIMPLE_REDIR_IN;
			else if (buf->id == REDIR_IN && ft_strlen(buf->token) == 2)
				buf->id = DOUBLE_REDIR_IN;
			else if (buf->id == REDIR_OUT && ft_strlen(buf->token) == 1)
				buf->id = SIMPLE_REDIR_OUT;
			else
				buf->id = DOUBLE_REDIR_OUT;
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	parser(void)
{
	if (are_quotes_even() == FAILURE)
		return (FAILURE);
	// replace_var
	if (check_pipes() == FAILURE)
		return (FAILURE);
	if (check_redir() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}