/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 23:15:47 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 10:02:36 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_ctx;

void	env_check(char **env)
{
	char	*pwd;

	pwd = NULL;
	if (!env[0])
	{
		pwd = getcwd(pwd, 0);
		ft_lstadd_back(&g_ctx.env, add_path_and_name_env(pwd, "PWD"));
		free (pwd);
	}
	else
		env_to_lst(env);
	// ------------------remove just print -----------------//
	// t_list *tmp = g_ctx.env;
	// while (tmp)
	// {
	// 	printf("NAME :%s\n", tmp->name_env);
	// 	printf("TOKEN :%s\n", tmp->path_env);
	// 	tmp = tmp->next;
	// }
	// ------------------remove just print -----------------//
}

void	env_to_lst(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_lstadd_back(&g_ctx.env, lst_new_env(env[i]));
		i++;
	}
}

t_list	*lst_new_env(char *str)
{
	t_list		*new;
	size_t		i;
	size_t		j;
	int			len;

	j = -1;
	i = -1;
	len = ft_strlen(str);
	new = malloc(sizeof(t_list));
	new->path_env = malloc(sizeof(char) * len);
	new->name_env = malloc(sizeof(char) * len);
	if (!new->path_env || !new->name_env || !new)
		return (NULL);
	while (str[++i] != '=')
		new->name_env[i] = str[i];
	while (str[++i])
	{
		new->path_env[++j] = str[i];
		new->name_env[i] = '\0';
	}
	while (new->path_env[j++])
		new->path_env[j] = '\0';
	new->next = NULL;
	return (new);
}

void	env_pwd_update(void)
{
	t_list	*temp;
	char	*pwd;
	int		i;

	i = -1;
	pwd = NULL;
	temp = g_ctx.env;
	pwd = getcwd(pwd, 0);
	while (!ft_strcmp(temp->name_env, "PWD"))
	{
		temp = temp->next;
	}
	free(temp->path_env);
	temp->path_env = malloc(sizeof(char) * ft_strlen(pwd));
	while (pwd[++i])
	{
		temp->path_env[i] = pwd[i];
	}
	free(pwd);
}

t_list	*add_var_env(char *token)
{
	t_list		*new;
	int			len_token;
	size_t		i;

	i = -1;
	len_token = ft_strlen(token);
	new = malloc(sizeof(t_list));
	new->token = malloc(sizeof(char) * len_token);
	if (!new->token || !new)
		return (NULL);
	while (token[++i])
		new->token[i] = token[i];
	new->id = 3;;
	new->next = NULL;
	return (new);
}
