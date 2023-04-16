/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   id.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:43:04 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 20:03:36 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_id(char c, int result)
{
	if (c == ' ' || c == 9 || c == 11)
		result = assign_id_to_char(c, WHITESPACE);
	else if (c == '\n')
		result = assign_id_to_char(c, BACKSLASH_N);
	else if (c == '$')
		result = assign_id_to_char(c, VAR);
	else if (c == '|')
		result = assign_id_to_char(c, PIPE);
	else if (c == '<')
		result = assign_id_to_char(c, REDIR_IN);
	else if (c == '>')
		result = assign_id_to_char(c, REDIR_OUT);
	else if (c == '\'')
		result = assign_id_to_char(c, SIMPLE_QUOTE);
	else if (c == '\"')
		result = assign_id_to_char(c, DOUBLE_QUOTE);
	else if (is_special_char(c) == TRUE && c != '_')
		result = assign_id_to_char(c, CHAR_SPE);
	else if (ft_isalnum(c) == TRUE || c == '_')
		result = assign_id_to_char(c, WORD);
	else
		return (FAILURE);
	return (result);
}

int	assign_id_to_char(char c, int id)
{
	t_lexer	*new;

	new = lex_lstnew();
	if (!new)
		return (FAILURE);
	new->c = c;
	new->id = id;
	if (g_all.lexer->c == 0)
	{
		free_one_lst_lexer(g_all.lexer);
		g_all.lexer = new;
	}
	else
		lex_lstadd_back(&g_all.lexer, new);
	return (SUCCESS);
}

char	*ft_add_char_to_str(char *str, char c)
{
	char	unique_char[2];
	char	*new;

	unique_char[0] = c;
	unique_char[1] = '\0';
	new = ms_strjoin(str, unique_char);
	if (str)
		free(str);
	if (!new)
		return (NULL);
	return (new);
}

void	update_id_spe_char(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == CHAR_SPE)
			buf->id = WORD;
		buf = buf->next;
	}
}

void	update_id_var(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == DOUBLE_QUOTE && ft_strchr(buf->token, '$'))
			buf->id = VAR;
		buf = buf->next;
	}
}
