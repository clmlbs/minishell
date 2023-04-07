/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 14:28:46 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/07 12:17:37 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_id_var(void)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == DOUBLE_QUOTE && ft_strchr(buf->token, '$'))
			buf->id = VAR;
		buf = buf->next;
	}
}

int	replace_var(void)
{
	t_lexer	*buf;
	int		i;

	i = 0;
	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == VAR)
		{
			while (buf->token[i])
			{
				if (buf->token[i] == '$')
				{
					if (update_token(buf, &i) == FAILURE)
						return (FAILURE);
					if (buf->token[0] == '\"')
						buf->id = DOUBLE_QUOTE;
					else
						buf->id = WORD;
				}
				i++;
			}
			i = 0;
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	update_token(t_lexer *lexer, int *index)
{
	char	*new;
	char	*var;
	int		i;
	int		begin;
	char 	*fullvar;
	char	*old_and_fullvar;

	new = NULL;
	i = 0;
	while (i < (*index))
	{
		new = ft_add_char_to_str(ft_strdup(new), lexer->token[i]);
		if (!new)
			return (FAILURE);
		i++;
	}
	i++; 
	begin = i;
	if (!lexer->token[i] || is_space(lexer->token[i]) == TRUE)
	{
		new = ft_add_char_to_str(ft_strdup(new), '$');
		if (!new)
			return (FAILURE);
		free(lexer->token);
		lexer->token = new;
		(*index)++;
		return (SUCCESS);
	}
	if (lexer->token[i] == '\"' && !(lexer->token[i + 1]))
	{
		new = ft_add_char_to_str(ft_strdup(new), '$');
		if (!new)
			return (FAILURE);
		new = ft_add_char_to_str(ft_strdup(new), lexer->token[i]);
		if (!new)
			return (FAILURE);
		free(lexer->token); // ok le free la ? 
		lexer->token = new;
		(*index)++;
		return (SUCCESS);
	}
	if (is_spe_or_num(lexer->token[i]) == TRUE)
		i++;
	else
	{
		i++;
		while (lexer->token[i] && (ft_isalnum(lexer->token[i]) == TRUE \
			|| lexer->token[i] == '_'))
			i++;
	}
	var = ft_substr(lexer->token, begin, i - begin);
	if (!var)
	{
		free(new);
		return (FAILURE);
	}
	fullvar = ft_getvar(var); // doit renvoyer soit str complete
							// soit NULL si echoue soit '\0' si n'existe pas	
	free(var);
	if (!fullvar)
	{
		free(new);
		return (FAILURE);
	}
	old_and_fullvar = ms_strjoin(new, fullvar);
	free(new);
	free(fullvar);
	if (!old_and_fullvar)
		return (FAILURE);
	(*index) = ft_strlen(old_and_fullvar);
	new = ms_strjoin(old_and_fullvar, \
		ft_substr(lexer->token, i, ft_strlen(lexer->token) - i));
	free(old_and_fullvar);
	if (!new)
		return (FAILURE);
	lexer->save = lexer->token;
	lexer->token = new;
	return (SUCCESS);
}