/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:15:19 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 10:31:48 by cleblais         ###   ########.fr       */
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

int	add_next_str_to_current(t_lexer *current)
{
	char	*new;

	new = ms_strjoin(current->token, current->next->token);
	if (!new)
		return (FAILURE);
	free(current->token);
	current->token = new;
	remove_token(current->next);
	return (SUCCESS);
}

int	parse_same_id(int id_target)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == id_target && buf->next && buf->next->id == id_target)
		{
			if (add_next_str_to_current(buf) == FAILURE)
				return (FAILURE);
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
	// replace_var : ne pas fair de tokn var mais word et remplacer la 
	//variable d'env avant d'enlever les quotes (car ne marche pas avec les ')
	// ce qui suit $ doit etre alphanumerique et ne pas commencer par un 
	// chiffre sinon ce n'est pas une var d'env 
	if (are_quotes_even() == FAILURE)
		return (FAILURE);
	parse_same_id(WORD);
	remove_spaces();
	if (parse_same_id(PIPE) == FAILURE || check_pipes() == FAILURE)
		return (FAILURE);
	if (parse_same_id(REDIR_IN) == FAILURE ||
		parse_same_id(REDIR_OUT) == FAILURE || check_redir() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}