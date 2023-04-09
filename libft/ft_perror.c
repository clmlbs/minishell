/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perror.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 16:23:10 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/09 16:23:23 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void perror_void(char *str)
{
	write(2, ORANGE, ft_strlen(ORANGE));
	perror(str);
	write(2, WHITE, ft_strlen(WHITE));
}