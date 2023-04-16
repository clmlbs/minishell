/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:56:13 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 15:17:21 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	var_name_len(char *str)
{
	int		i;
	char	c;

	c = '=';
	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}

void	change_var_value(char *str, int *env_index)
{
	char	*buf;

	buf = ft_strdup(str);
	if (!buf)
		return ;
	free(g_all.env[*env_index]);
	g_all.env[*env_index] = buf;
	buf = NULL;
}

void	put_in_alphabetic_order(char **strs)
{
	int		i;
	int		j;	
	int		len;
	char	*temp;

	len = tab_strlen(strs);
	i = 0;
	j = 0;
	while (i < len - 1)
	{
		while (j < len - i - 1)
		{
			if (ft_strcmp(strs[j], strs[j + 1]) > 0)
			{
				temp = strs[j];
				strs[j] = strs[j + 1];
				strs[j + 1] = temp;
			}
			j++;
		}
		j = 0;
		i++;
	}
}