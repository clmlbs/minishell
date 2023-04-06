/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 18:41:46 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/06 11:53:13 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_unset(t_cmd	*cmd)
{
	char	**new;
	int		var_target;
	int		index;

	var_target = 1;
	index = 0;
	while (cmd->wd[var_target])
	{
		new = copy_strs_plus_one(g_all.env);
		if (!new)
			exit(FAILURE);
		if (is_var_exist(cmd->wd[var_target], &index) == SUCCESS)
		{
			new = remove_var(new, index, 0); // penser a free ancien new dedans
			if (!new)
				exit(FAILURE);
			free_tab_strs(g_all.env);
			g_all.env = new;
			new = NULL;
		}
		index = 0;
		var_target++;
	}
	if (new)
		free_tab_strs(new);
	if (send_env_to_father(g_all.env, g_all.herit) == FAILURE)
		exit(FAILURE);
	printf_strs(g_all.env, WITH_INDEX, 1);//*****
	exit(SUCCESS);
}

// int	send_env_to_father(char **env, int *fd)
// {
// 	int		*nb_strs;
// 	size_t	len;
// 	int		i;

// 	i = 0;
// 	if (close(g_all.size[0]) < 0 || close(fd[0] < 0))
// 		return (perror_fail("Minishell: close()"));
// 	nb_strs = (int*)malloc(sizeof(int));
// 	if (!nb_strs)
// 		return (perror_fail("Minishell: malloc()"));
// 	nb_strs[0] = tab_strlen(env);
// 	printf("nb_strs[0]:%d\n", nb_strs[0]);//**********
// 	if (write(g_all.size[1], nb_strs, sizeof(int)) == -1)
// 		return (perror_fail("Minishell: write()"));
// 	while (env[i])
// 	{
// 		len = ft_strlen(env[i]) + 1;
// 		if (write(fd[1], env[i], len) == -1)
// 			return (perror_fail("Minishell: write()"));
// 		i++;
// 	}
// 	return (SUCCESS);
// }

int send_env_to_father(char **env, int *fd) {
    int *nb_strs;
    size_t len;
    int i;

    if (close(g_all.size[0]) < 0 || close(fd[0]) < 0)
        return (perror_fail("Minishell: close()"));
    nb_strs = (int*)malloc(sizeof(int));
    if (!nb_strs)
        return (perror_fail("Minishell: malloc()"));
    nb_strs[0] = tab_strlen(env);
    if (write(g_all.size[1], nb_strs, sizeof(int)) == -1)
        return (perror_fail("Minishell: write()"));
    i = 0;
    while (env[i]) {
        len = ft_strlen(env[i]) + 1;
        if (write(fd[1], &len, sizeof(size_t)) == -1)
            return (perror_fail("Minishell: write()"));
        if (write(fd[1], env[i], len) == -1)
            return (perror_fail("Minishell: write()"));
        i++;
    }
    free(nb_strs);
    return (SUCCESS);
}


char	**remove_var(char **env, int index, int i)
{
	char	**new;

	new = (char **)malloc(sizeof(char *) * tab_strlen(env) + 1);
	if (!new)
		return (error_strdup(env, new));
	while (env[i] && i < index)
	{
		new[i] = ft_strdup(env[i]);
		if (!new[i])
			return (error_strdup(env, new));
		i++;
	}
	while (env[i + 1])
	{
		new[i] = ft_strdup(env[i + 1]);
		if (!new[i])
			return (error_strdup(env, new));
		i++;
	}
	new[i] = NULL;
	free_tab_strs(env);
	return (new);
}

char	**error_strdup(char **env, char **new)
{
	perror("Minishell: malloc()");
	free_tab_strs(env);
	if (new)
		free_tab_strs(new);
	return (NULL);
}