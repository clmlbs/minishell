/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:15:19 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 11:24:48 by cleblais         ###   ########.fr       */
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

void	change_id_redir(t_lexer *lst)
{
	if (lst->id == REDIR_IN && ft_strlen(lst->token) == 1)
		lst->id = SIMPLE_REDIR_IN;
	else if (lst->id == REDIR_IN && ft_strlen(lst->token) == 2)
		lst->id = DOUBLE_REDIR_IN;
	else if (lst->id == REDIR_OUT && ft_strlen(lst->token) == 1)
		lst->id = SIMPLE_REDIR_OUT;
	else
		lst->id = DOUBLE_REDIR_OUT;
}


int	check_redir(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if ((buf->id == REDIR_IN || buf->id == REDIR_OUT) && !buf->next)
		{
			write_error("Minishell: ", "syntax error near", \
			" unexpected token `newline'\n");
			return (FAILURE);
		}
		if (buf->id == REDIR_IN || buf->id == REDIR_OUT)
		{
			if (ft_strlen(buf->token) < 1 || ft_strlen(buf->token) > 2)
			{
				error_token(buf, ft_strlen(buf->token));
				return (FAILURE);
			}
			change_id_redir(buf);
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	parse_token_after_redir(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id >= SIMPLE_REDIR_IN && buf->id <= DOUBLE_REDIR_OUT)
		{
			if (buf->id == SIMPLE_REDIR_IN && buf->next && buf->next->id == WORD)
				buf->next->id = INFILE_NAME;
			else if (buf->id == DOUBLE_REDIR_IN && buf->next && \
				buf->next->id == WORD)
				buf->next->id = KEY_WORD_HERE_DOC;
			else if ((buf->id == SIMPLE_REDIR_OUT || buf->id == DOUBLE_REDIR_OUT) \
				&& buf->next && buf->next->id == WORD)
				buf->next->id = OUTFILE_NAME;
			else
			{
				write_error("Minishell: ", "syntax error", "\n");
				return (FAILURE);
			}
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
	if (parse_token_after_redir() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}