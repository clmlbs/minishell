/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 11:06:23 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/03 14:49:34 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_all;

void	write_error(char *begin, char *middle, char *end)
{
	write(2, ORANGE, ft_strlen(ORANGE));
	write(2, begin, ft_strlen(begin));
	write(2, WHITE, ft_strlen(WHITE));
	write(2, middle, ft_strlen(middle));
	write(2, end, ft_strlen(end));
}

void	error_token(t_lexer *lst, int len)
{
	if (len == 2 || len == 3) //|| lst->token[3] != lst->token[4])
	{
		write_error("Minishell:", " syntax error near unexpected", " token `");
		write(2, &(lst->token[len - 1]), 1);
		write(2, "'\n", 2);
	}
	else if (len > 3)
	{
		write_error("Minishell:", " syntax error near unexpected", " token `");
		write(2, &(lst->token[2]), 1);
		write(2, &(lst->token[3]), 1);
		write(2, "'\n", 2);
	}
}

int	perror_failure(char *str)
{
	write(2, ORANGE, ft_strlen(ORANGE));
	perror(str);
	write(2, WHITE, ft_strlen(WHITE));
	return (FAILURE);
}

void perror_void(char *str)
{
	write(2, ORANGE, ft_strlen(ORANGE));
	perror(str);
	write(2, WHITE, ft_strlen(WHITE));
}