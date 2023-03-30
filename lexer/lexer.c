/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 10:55:11 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_ctx;

void	lexer(char *input)
{
	char	**argv;

	argv = ft_split(input, ' ');
	if (!argv)
		return (perror("Minishell: lexer()"));
	init_token_id(argv);
	free_tab_strs(argv);
	if (formate_token_id() == FAILURE) // sert a mettre un id en fonction du type de caracteres spe / fichier etc
		return ;
		
	t_list *tmp = g_ctx.lexer;
	while (tmp)
	{
		printf("TOKEN :%s\n", tmp->token);
		printf("TOKEN_ID :%d\n\n", tmp->id);
		tmp = tmp->next;
	}
	printf("Nb cmd = %d\n", g_ctx.nb_cmd);//***
	parser(-1);
}

void	init_token_id(char **argv)
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
				ft_lstadd_back(&g_ctx.lexer, add_token(temp, 1));
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
				ft_lstadd_back(&g_ctx.lexer, add_var_env(temp));
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
				ft_lstadd_back(&g_ctx.lexer, add_token(temp, 0));
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
