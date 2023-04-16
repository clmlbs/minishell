/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:18:59 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 16:19:34 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_is_present(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	start;
	size_t			len;

	if (!s1)
		return (NULL);
	i = 0;
	if (s1[i])
	{
		while (ft_is_present(set, s1[i]) == 1 && s1[i])
			i++;
		start = i;
		i = ms_strlen(s1) - 1;
		while (s1 && ft_is_present(set, s1[i]) == 1 && i > 0)
			i--;
		len = ms_strlen(s1) - start - (ms_strlen(s1) - i) + 1;
		return (ft_substr(s1, start, len));
	}
	return (ft_substr(s1, 0, 0));
}
