/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/04 17:54:55 by cleblais         ###   ########.fr       */
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

char	*ft_getvar(char *var)
{
	int		i;
	int		j;
	int		exist;
	char	*new;

	exist = NO;
	i = 0;
	j = 0;
	while (g_all.env[i])
	{
		if (!ft_strncmp(g_all.env[i], var, ft_strlen(var)))
		{
			while (g_all.env[i][j] && g_all.env[i][j] != '=')
				j++;
			if (j == ft_strlen(var))
			{	
				exist = YES;
				break ;
			}
			j = 0;
		}
		i++;
	}
	if (exist == NO)
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