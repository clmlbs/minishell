/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 13:33:39 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 13:36:51 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_all;

void built_env(void)
{
    t_list  *temp;

    temp = g_all.env;
    while (!ft_strcmp(temp->name_env, "PWD"))
	{
		temp = temp->next;
	}
    printf("%s\n", temp->path_env);
}