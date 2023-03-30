/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 14:22:40 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_all;

t_list	*new_lexer(void)
{
	t_list	*lexer;

	lexer = (t_list *)malloc(sizeof(t_list));
	if (!lexer)
	{
		perror("Minishell: malloc()");
		return (NULL);
	}
	lexer->prev = NULL;
	lexer->token = NULL;
	lexer->id = WORD;
	lexer->next = NULL;
	return (lexer);
}

void	lexer(char *input)
{
	g_all.lexer = NULL;
	if (init_token_id(input) == FAILURE)
		return ;
	if (formate_token_id() == FAILURE) // sert a mettre un id en fonction du type de caracteres spe / fichier etc
		return ;
		
	t_list *tmp = g_all.lexer;
	while (tmp)
	{
		printf("TOKEN :%s\n", tmp->token);
		printf("TOKEN_ID :%d\n\n", tmp->id);
		tmp = tmp->next;
	}
	printf("Nb cmd = %d\n", g_all.nb_cmd);//***
	parser(-1);
}

int	token_redir_or_pipe(char *input, int *index)
{
	int		i;
	t_list	*buf;

	i = 0;
	while (input[*index + i] && (is_redir_or_pipe(input[*index + i]) || \
			input[*index + i] == ' '))
		i++;
	buf = new_lexer();
	if (!buf)
		return (FAILURE);
	buf->id = 1;
	buf->token = ft_strtrim(ft_substr(input, index, i + 1), " ");
	if (!buf->token)
	{	
		free(buf);
		return (FAILURE);
	}
	ft_lstadd_back(&g_all.lexer, buf);
	*index += i;
	return (SUCCESS);
}

int	init_token_id(char *input)
{
	int	i;
	int	result;

	i = 0;
	result = SUCCESS;
	while (input[i])
	{
		if (is_redir_or_pipe(input[i]))
			result = token_redir_or_pipe(input, &i);
		else if (input[i] == 39)
			result = token_simple_quote(input, &i);
		else if (input[i] == 34)
			result = token_double_quote(input, &i);
		else if (input[i] == '$')
			result = token_var(input, &i);
		else if (isnt_specific(input[i]) == SUCCESS)
			result = token_unspecific(input, &i);
		else
			i++;
		if (result == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

void	init_token_id_nope(char **argv)
{
	char	*temp;
	int		i;
	int		j;
	int 	x = 0;
	int 	k = 0;

	j = 0;
	i = 0;
	while (argv[i])
	{
		while (argv[i][j])
		{
			if (ft_isalpip(argv[i][j]))
			{
				while (ft_isalpip(argv[i][j]))
				{
					x++;
					j++;
				}
				printf("%d\n", x);
				temp = malloc(sizeof(char) * x);
				while (x)
				{
					temp[k] = argv[i][j - x];
					x--;
					k++;
				}
				ft_lstadd_back(&g_all.lexer, add_token(temp, 1));
				free(temp);
			}
			else if (ft_isavar(argv[i][j]))
			{
				while (ft_isavar(argv[i][j]))
				{
					x++;
					j++;
				}
				temp = malloc(sizeof(char) * x);
				while (x)
				{
					temp[k] = argv[i][j - x];
					x--;
					k++;
				}
				ft_lstadd_back(&g_all.lexer, add_var_env(temp));
				free(temp);
			}
			else if (ft_isanum(argv[i][j]))
			{
				while (ft_isanum(argv[i][j]))
				{
					x++;
					j++;
				}
				temp = malloc(sizeof(char) * x);
				while (x)
				{
					temp[k] = argv[i][j - x];
					x--;
					k++;
				}
				ft_lstadd_back(&g_all.lexer, add_token(temp, 0));
				free(temp);
			}
			else
				j++;
			x = 0;
			k = 0;
		}
	j = 0;
	i++;
	}
}
