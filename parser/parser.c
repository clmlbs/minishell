/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:15:19 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/31 14:22:59 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_pipes(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == PIPE)
		{
			if (ft_strlen(buf->token) != 1)
			{
				write_error("Minishell: ", \
				"error: more than one pipe `|' are not taken in charge" ,\
				" in this Minishell\n");
				return (FAILURE);
			}
		}
		buf = buf->next;
	}
	return (SUCCESS);
}


int	parser(void)
{
	if (are_quotes_even() == FAILURE)
		return (FAILURE);
	// replace_var
	if (check_pipes() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}