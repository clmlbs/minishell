/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 11:56:13 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 17:31:57 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*put_to_lower(char *str)
{
	char	*new;
	int		i;

	i = 0;
	new = ft_strdup(str);
	if (!new)
		return (NULL);
	while (new[i])
	{
		new[i] = ft_tolower(new[i]);
		i++;
	}
	return (new);
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
