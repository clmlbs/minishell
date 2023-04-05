/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/05 16:57:58 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_all;

int	lexer(char *input)
{
	int	i;

	i = 0;
	g_all.lexer = lex_lstnew();
	if (!g_all.lexer)
		return (FAILURE);
	//printf("input:%s\n", input);//*****
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
		if (!ft_strncmp(g_all.env[*index], var, ft_strlen(var)))
		{
			while (g_all.env[*index][j] && g_all.env[*index][j] != '=')
				j++;
			if (j == ft_strlen(var))
				return (SUCCESS);
			j = 0;
		}
		(*index)++;
	}
	return (FAILURE);
}

char	*ft_getvar(char *var)
{
	int		i;

	char	*new;

	i = 0;
	if (is_var_exist(var, &i) == FAILURE)
	{
		new = (char *)malloc(sizeof(char));
		if (!new)
		{
			perror("Minishell");
			return (NULL);
		}
		new[0] = '\0';
		return (new);
	}
	new = ft_strdup(g_all.env[i] + ft_strlen(var) + 1);
	if (!new)
		return (NULL);
	return (new);
}

int	tokenize_all_steps(void)
{
	if (tokenize_quotes(NO, 0) == FAILURE) // verif si ok
		return (FAILURE);
	if (tokenize_words(VAR, WORD) == FAILURE)
		return (FAILURE);
	update_id_var();
	if (replace_var() == FAILURE)
		return (FAILURE);
	//print_t_lexer();
	if (tokenize_words(WORD, WORD) == FAILURE)
		return (FAILURE);
	if (tokenize_words(PIPE, PIPE) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_IN, REDIR_IN) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_OUT, REDIR_OUT) == FAILURE)
		return (FAILURE);
	//print_t_lexer();//******************
	return (SUCCESS);
}