/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strzero.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:11:41 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/09 16:18:29 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strzero(char *s, size_t n)
{
	size_t		i;
	char		*ptr;

	i = 0;
	if (!s)
		return (NULL);
	ptr = s;
	while (i < n)
	{
		ptr[i] = '\0';
		i++;
	}
	return (s);
}
