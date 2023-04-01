/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_function.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 19:29:39 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 10:46:23 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_all;

// t_lexer	*add_path_and_name_env(char *path, char *name)
// {
// 	t_lexer		*new;
// 	int			len_name;
// 	int			len_path;
// 	size_t		i;

// 	i = -1;
// 	len_name = ft_strlen(name);
// 	len_path = ft_strlen(path);
// 	new = malloc(sizeof(t_lexer));
// 	new->path_env = malloc(sizeof(char) * len_path);
// 	new->name_env = malloc(sizeof(char) * len_name);
// 	if (!new->path_env || !new->name_env || !new)
// 		return (NULL);
// 	while (path[++i])
// 		new->path_env[i] = path[i];
// 	i = -1;
// 	while (name[++i])
// 		new->name_env[i] = name[i];
// 	new->next = NULL;
// 	return (new);
// }

// t_lexer	*add_token(char *token, int id)
// {
// 	t_lexer		*new;
// 	int			len_token;
// 	size_t		i;

// 	i = -1;
// 	len_token = ft_strlen(token);
// 	new = malloc(sizeof(t_lexer));
// 	new->token = malloc(sizeof(char) * len_token);
// 	if (!new->token || !new)
// 		return (NULL);
// 	while (token[++i])
// 		new->token[i] = token[i];
// 	new->id = id;
// 	new->next = NULL;
// 	return (new);
// }
