/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:42:44 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 22:38:12 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	create_var_name(t_lexer *lexer, t_update_token *t)
{
	t->var_name = ft_substr(lexer->token, t->begin, t->i - t->begin);
	if (!(t->var_name))
	{
		free(t->str_begin);
		t->str_begin = NULL;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	is_var(char c)
{
	if (c == '$')
		return (1);
	return (0);
}
