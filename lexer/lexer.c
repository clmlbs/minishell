/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 15:13:20 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	lexer(char *input)
{
	int	i;

	i = 0;
	g_all.lexer = lex_lstnew();
	if (!g_all.lexer)
		return (FAILURE);
	while (input[i])
	{
		if (init_id(input[i]) == FAILURE)
		{
			write_error("Minishell: ", "syntax error", "\n");
			return (FAILURE);
		}
		i++;
	}
	if (tokenize_all_steps() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	is_var_exist(char *var, int *index)
{
	int	j;

	j = 0;
	while (g_all.env[*index])
	{
		if (!ft_strncmp(g_all.env[*index], var, ms_strlen(var)))
		{
			while (g_all.env[*index][j] && g_all.env[*index][j] != '=')
				j++;
			if (j == ms_strlen(var))
				return (SUCCESS);
			j = 0;
		}
		(*index)++;
	}
	return (FAILURE);
}


// MARCHE
// int	is_var_exist(char *var, int *index)
// {
// 	int	j;

// 	j = 0;
// 	while (g_all.env[*index])
// 	{
// 		if (!ft_strncmp(g_all.env[*index], var, ms_strlen(var)))
// 		{
// 			while (g_all.env[*index][j] && g_all.env[*index][j] != '=')
// 				j++;
// 			if (j == ms_strlen(var))
// 				return (SUCCESS);
// 			j = 0;
// 		}
// 		(*index)++;
// 	}
// 	return (FAILURE);
// }

char *var_is_exit_status(void)
{
	char	*new;

	new = ft_itoa(g_all.status);
	if (!new)
	{
		perror("Minishell");
		return (NULL);
	}
	return (new);
}

char	*create_var_value(char *var)
{
	char	*new;
	int		i;

	i = 0;
	if (is_var_exist(var, &i) == FAILURE)
	{
		if (ms_strlen(var) == 1 && !ft_strncmp("?", var, 1))
			return (var_is_exit_status());
		new = (char *)malloc(sizeof(char));
		if (!new)
			perror("Minishell");
		if (!new)
			return (NULL);
		new[0] = '\0';
		return (new);
	}
	new = ft_strdup(g_all.env[i] + ms_strlen(var) + 1);
	if (!new)
		return (NULL);
	return (new);
}

void	update_id_spe_char(void)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == CHAR_SPE)
			buf->id = WORD;
		buf = buf->next;
	}
}

int	tokenize_exit_status_var(void)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == VAR && ms_strlen(buf->token) == 1 && buf->next \
			&& buf->next->c == '?' && buf->next->token == NULL)
		{
			if (add_next_char_to_current(buf) == FAILURE)
					return (FAILURE);	
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	tokenize_all_steps(void)
{
	if (tokenize_quotes(NO, 0) == FAILURE)
		return (FAILURE);
	if (tokenize_words(VAR, WORD) == FAILURE)
		return (FAILURE);
	if (tokenize_exit_status_var() == FAILURE)
		return (FAILURE);
	update_id_var();
	//print_t_lexer();//********
	if (replace_var() == FAILURE)
		return (FAILURE);
	//print_t_lexer();//********
	update_id_spe_char();
	if (tokenize_words(WORD, WORD) == FAILURE)
		return (FAILURE);
	if (tokenize_words(PIPE, PIPE) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_IN, REDIR_IN) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_OUT, REDIR_OUT) == FAILURE)
		return (FAILURE);
	//print_t_lexer();//*******
	return (SUCCESS);
}