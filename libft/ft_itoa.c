/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:13:01 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 16:06:42 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_bzero(void *s, size_t n)
{
	size_t		i;
	char		*ptr;

	i = 0;
	ptr = s;
	while (i < n)
	{
		ptr[i] = '\0';
		i++;
	}
}

static char	*ft_create_str(long int n)
{
	long int	neg;
	long int	pow;
	long int	nb;
	char		*str;

	pow = 1;
	if (n < 0)
	{
		neg = 1;
		n = -n;
	}
	else
		neg = 0;
	nb = n;
	while (nb >= 10)
	{
		pow++;
		nb = nb / 10;
	}
	str = (char *)malloc(sizeof(*str) * (pow + neg + 1));
	if (!str)
		return (NULL);
	ft_bzero(str, pow + neg + 1);
	return (str);
}

static void	ft_write_nb(long int n, int *index, char *str)
{
	if (n < 10)
		str[*index] = n + '0';
	else
	{
		ft_write_nb(n / 10, index, str);
		(*index)++;
		ft_write_nb(n % 10, index, str);
	}
}

char	*ft_itoa(int n)
{
	char		*str;
	int			i;
	long int	nb;

	nb = (long int)n;
	str = ft_create_str(nb);
	if (!str)
		return (NULL);
	i = 0;
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
		i++;
	}
	ft_write_nb(nb, &i, str);
	return (str);
}
