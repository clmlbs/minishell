/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:34:12 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/03 14:46:38 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *s1)
{
	char	*new_str;

	new_str = malloc(sizeof(*new_str) * (ft_strlen(s1) + 1));
	if (!new_str)
	{
		perror_void("Minishell: malloc()");
		return (NULL);
	}
	ft_strlcpy(new_str, s1, ft_strlen(s1) + 1);
	return (new_str);
}