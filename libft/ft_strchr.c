/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:17:01 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/11 11:43:57 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	uc_c;
	char			*str;
	int				i;

	uc_c = c;
	str = (char *)s;
	i = 0;
	if (!s)
		return (NULL);
	if (c == 0)
		return (str + ms_strlen(s));
	while (str[i])
	{
		if (str[i] == uc_c)
			return (&str[i]);
		i++;
	}
	if (uc_c == '\0')
		return (&str[i]);
	else
		return (NULL);
}
