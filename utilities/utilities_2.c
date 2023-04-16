/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 22:33:13 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 22:43:17 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special_char_or_numeric(char c)
{
	if (c >= 33 && c <= 64)
		return (TRUE);
	if ((c >= 91 && c <= 96) && c != '_')
		return (TRUE);
	if (c <= 123 && c >= 127)
		return (TRUE);
	return (FALSE);
}

int	is_special_char(char c)
{
	if (c == '_')
		return (FALSE);
	if (c == '!' || c == '#' || c == '%' || c == '&')
		return (TRUE);
	if (c >= '(' && c <= '/')
		return (TRUE);
	if (c == ':' || c == ';' || c == '=')
		return (TRUE);
	if (c >= '?' && c <= '@')
		return (TRUE);
	if (c >= '[' && c <= '`')
		return (TRUE);
	if (c == '{' || c == '}' || c == '~')
		return (TRUE);
	return (FALSE);
}

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
		return (TRUE);
	else
		return (FALSE);
}

void	printf_strs(char **strs, int mode, int fd)
{
	int	i;

	i = 0;
	if (strs)
	{
		if (mode == WITH_INDEX)
		{
			while (strs[i])
			{
				printf("%d] %s\n", i, strs[i]);
				i++;
			}
		}
		else
		{
			while (strs[i])
			{
				ft_putstr_fd(strs[i], fd);
				ft_putstr_fd("\n", fd);
				i++;
			}
		}
	}
}

int	tab_strlen(char **tab_of_str)
{
	int	i;

	if (tab_of_str == NULL || tab_of_str[0] == NULL)
		return (0);
	i = 0;
	while (tab_of_str[i])
		i++;
	return (i);
}
