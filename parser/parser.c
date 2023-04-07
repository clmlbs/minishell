/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:15:19 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/07 12:34:37 by cleblais         ###   ########.fr       */
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
		if (buf->id == SIMPLE_REDIR_IN && buf->next && buf->next->id == WORD)
			buf->next->id = INFILE_NAME;
		else if (buf->id == DOUBLE_REDIR_IN && buf->next && \
			buf->next->id == WORD)
			buf->next->id = KEY_WORD_HERE_DOC;
		else if ((buf->id == SIMPLE_REDIR_OUT || buf->id == DOUBLE_REDIR_OUT) \
			&& buf->next && buf->next->id == WORD)
			buf->next->id = OUTFILE_NAME;
		else if (buf->id >= SIMPLE_REDIR_IN && buf->id <= DOUBLE_REDIR_OUT)
		{
			write_error("Minishell: ", "syntax error", "\n");
			return (FAILURE);
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

void	key_word_contain_dollar(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == KEY_WORD_HERE_DOC)
		{
			if (buf->save && buf->save[0] == '$')
			{
				free(buf->token);
				buf->token = buf->save;
				buf->save = NULL;
			}
		}
		buf = buf->next;
	}
}

int	parser(void)
{
	// replace_var : ne pas fair de tokn var mais word et remplacer la 
	//variable d'env avant d'enlever les quotes (car ne marche pas avec les ')
	// ce qui suit $ doit etre alphanumerique et ne pas commencer par un 
	// chiffre sinon ce n'est pas une var d'env 
	if (are_quotes_even() == FAILURE)
		return (FAILURE);
	if (parse_same_id(WORD) == FAILURE)
		return (FAILURE);
	remove_spaces(); 
	if (parse_same_id(PIPE) == FAILURE || check_pipes() == FAILURE)
		return (FAILURE);
	if (parse_same_id(REDIR_IN) == FAILURE ||
		parse_same_id(REDIR_OUT) == FAILURE || check_redir() == FAILURE)
		return (FAILURE);
	if (parse_token_after_redir() == FAILURE)
		return (FAILURE);
	key_word_contain_dollar();
	return (SUCCESS);
}