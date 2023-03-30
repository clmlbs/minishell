/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:05:21 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 09:40:15 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_isanum(char c)
{
	if (c >= 33 && c <= 126 && (!ft_isalpip(c)
			&& !ft_isavar(c)))
		return (1);
	return (0);
}

int	ft_isalpip(char c)
{
	if ((c == '<')
		|| (c == '>' || c == '|'))
		return (1);
	return (0);
}

int	ft_isavar(char c)
{
	if (c == '$')
		return (1);
	return (0);
}

void	printf_strs(char **strs)
{
	int	i;
	
	i = 0;
	while (strs[i])
	{
		printf("%d] %s\n", i, strs[i]);
		i++;
	}
}

int	copy_tab_of_strs(char **old, char **new)
{
	int tab_len;
	int	i;

	tab_len = tab_strlen(old);
	i = 0;
	while (i < tab_len)
	{
		new[i] = ft_strdup(old[i]);
		if (!new[i])
		{
			perror("Minishell: ft_strdup()");
			free_tab_strs(new);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

int	tab_strlen(char **tab_of_str)
{
	int i;

	if (tab_of_str == NULL || tab_of_str[0] == NULL)
		return (0);
	i = 0;
	while (tab_of_str[i])
		i++;
	return (i);
}