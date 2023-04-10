/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 11:06:23 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/10 07:43:31 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_error(char *begin, char *middle, char *end)
{
	write(2, begin, ft_strlen(begin));
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

int	perror_fail(char *str)
{
	perror(str);
	return (FAILURE);
}