/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:15:19 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/14 21:37:26 by cleblais         ###   ########.fr       */
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
			if (ms_strlen(buf->token) != 1)
			{
				error_token(buf, ms_strlen(buf->token), PIPE);
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
			if (ms_strlen(buf->token) < 1 || ms_strlen(buf->token) > 2)
			{
				error_token(buf, ms_strlen(buf->token), REDIR);
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

int	check_pipes_begin(void)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->prev == NULL && buf->id == PIPE)
		{
			write_error("Minishell: syntax error near unexpected ", \
			"token `|'", "\n");
			return (FAILURE);
		}
		buf = buf->next;
	}
	buf = lex_lstlast(g_all.lexer);
	if (buf->id == PIPE)
	{
		write_error("Minishell: syntax error near unexpected ", \
			"token `|'", "\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	nb_redir(int id_target)
{
	t_lexer *buf;
	int		nb;

	nb = 0;
	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == id_target)
			nb++;
		buf = buf->next;
	}
	return (nb);
}

int	parse_redir(void)
{
	int		nb_redir_in;
	int		nb_redir_out;

	if (parse_same_id(REDIR_IN) == 1 || parse_same_id(REDIR_OUT) == FAILURE)
		return (FAILURE);
	nb_redir_in = nb_redir(REDIR_IN);
	nb_redir_out = nb_redir(REDIR_OUT);
	remove_spaces();
	if (parse_same_id(REDIR_IN) == 1 || parse_same_id(REDIR_OUT) == FAILURE)
		return (FAILURE);
	if (nb_redir_in != nb_redir(REDIR_IN))
	{
		write_error("Minishell: syntax error near unexpected token ", \
			"`<'", "\n");
		return (FAILURE);
	}
	if (nb_redir_out != nb_redir(REDIR_OUT))
	{
		write_error("Minishell: syntax error near unexpected token ", \
			"`>'", "\n");
		return (FAILURE);
	}
	if (check_redir() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

void	suppr_useless_token(void)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == SUPPR)
		{
			remove_token(buf);
			buf = g_all.lexer;			
		}
		else
			buf = buf->next;
	}
}

int	parser(void)
{
	if (are_quotes_even() == FAILURE)
		return (FAILURE);
	suppr_useless_token();
	//print_t_lexer();//********
	if (parse_same_id(WORD) == FAILURE)
		return (FAILURE);
	//print_t_lexer();//********
	if (parse_redir() == FAILURE)
		return (FAILURE);
	if (parse_same_id(PIPE) == FAILURE || check_pipes() == FAILURE)
		return (FAILURE);
	if (parse_token_after_redir() == FAILURE)
		return (FAILURE);
	key_word_contain_dollar();
	if (check_pipes_begin() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}