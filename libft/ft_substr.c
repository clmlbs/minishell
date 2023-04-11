/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:19:10 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/11 11:43:57 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	if (ms_strlen(s) == 0)
		len = 0;
	if (len > ms_strlen(s) - start)
		len = ms_strlen(s) - start;
	if (start > ms_strlen(s) - 1)
		len = 0;
	i = 0;
	str = (char *)malloc(sizeof(*str) * (len + 1));
	if (!str)
	{
		perror_void("Minishell: malloc()");
		return (NULL);
	}
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
