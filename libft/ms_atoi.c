/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:11:27 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/09 16:18:59 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

long long	ms_atoi(const char *str)
{
	int			i;
	int			sign;
	long long	total;

	i = 0;
	sign = 1;
	total = 0;
	while (str[i] && (str[i] == ' ' || (str[i] > 8 && str[i] < 14)))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		total = total * 10 + (str[i] - '0');
		i++;
	}
	return ((long long) total * sign);
}
