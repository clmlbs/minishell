/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:53:23 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 21:09:04 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_t_updated_token(t_update_token *t)
{
	t->i = 0;
	t->begin = 0;
	t->str_begin = NULL;
	t->var_name = NULL;
	t->var_value = NULL;
	t->begin_and_value = NULL;
	t->token_updated = NULL;
}

char	*var_is_exit_status(void)
{
	char	*new;

	new = ft_itoa(g_all.status);
	if (!new)
	{
		perror("Minishell");
		return (NULL);
	}
	return (new);
}
