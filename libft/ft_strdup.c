/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:34:12 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 16:18:40 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strdup(const char *s1)
{
	char	*new_str;

	if (!s1)
		return (NULL);
	new_str = malloc(sizeof(*new_str) * (ms_strlen(s1) + 1));
	if (!new_str)
	{
		perror_void("Minishell: malloc()");
		return (NULL);
	}
	ft_strlcpy(new_str, s1, ms_strlen(s1) + 1);
	return (new_str);
}