/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:05:21 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 22:42:43 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	isnt_specific(char c)
{
	if (c >= 33 && c <= 126 && !is_redir_or_pipe(c) && !is_var(c) \
		&& !is_quote(c))
		return (SUCCESS);
	return (FAILURE);
}

int	is_redir_or_pipe(char c)
{
	if ((c == '<') || (c == '>' || c == '|'))
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == 34 || c == 39)
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == '\n' || c == 9 || c == 11 || c == ' ')
		return (TRUE);
	return (FALSE);
}

void	print_t_lexer(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		ft_putstr_fd("id: ", 2);
		ft_putnbr_fd(buf->id, 2);
		ft_putstr_fd("		", 2);
		ft_putchar_fd(buf->c, 2);
		ft_putstr_fd("	", 2);
		ft_putstr_fd(buf->token, 2);
		if (buf->save)
		{
			ft_putstr_fd("	", 2);
			ft_putstr_fd(buf->save, 2);
		}
		ft_putstr_fd("\n", 2);
		buf = buf->next;
	}
	ft_putstr_fd("\n", 2);
}
