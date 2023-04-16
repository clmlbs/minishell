/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:17:27 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 16:18:47 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ms_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		len;
	char	buf[1];

	if (!s1)
	{
		buf[0] = '\0';
		s1 = buf;
	}
	len = ms_strlen(s1) + ms_strlen(s2) + 1;
	str = (char *)malloc(sizeof(*str) * len);
	if (!str)
	{
		perror_void("Minishell: malloc()");
		return (NULL);
	}
	ft_strlcpy(str, s1, ms_strlen(s1) + 1);
	ft_strlcat(str, s2, ms_strlen(s1) + ms_strlen(s2) + 1);
	return (str);
}
