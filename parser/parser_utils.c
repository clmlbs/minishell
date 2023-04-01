/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 10:47:51 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 10:48:03 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_spaces(void)
{
	t_lexer	*buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == WHITESPACE || buf->id == BACKSLASH_N)
			remove_token(buf);
		buf = buf->next;
	}
}