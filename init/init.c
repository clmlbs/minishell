/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:32:31 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/03 12:10:15 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_global(char **av)
{
	(void)av;
	g_all.lexer = NULL;
	g_all.cmd = cmd_lstnew();
	if (!g_all.cmd)
		exit(1);// bon status de sortie ? 
	g_all.all_path = NULL;
	g_all.env = NULL;
	g_all.nb_cmd = 0;
}

