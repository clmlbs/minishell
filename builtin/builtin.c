/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 08:44:20 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 17:11:45 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(t_cmd *cmd)
{
	char	*str;
	int		result;

	result = FALSE;
	str = put_to_lower(cmd->wd[0]);
	if (!str)
		return (FALSE);
	if (ms_strlen(str) == 2 && !ft_strncmp(str, "cd", 2))
		result = TRUE;
	else if (ms_strlen(str) == 3 && !ft_strncmp(str, "pwd", 3))
		result = TRUE;
	else if (ms_strlen(str) == 3 && !ft_strncmp(str, "env", 3))
		result = TRUE;
	else if (ms_strlen(str) == 4 && !ft_strncmp(str, "echo", 4))
		result = TRUE;
	else if (ms_strlen(str) == 4 && !ft_strncmp(str, "exit", 4))
		result = TRUE;
	else if (ms_strlen(str) == 5 && !ft_strncmp(str, "unset", 5))
		result = TRUE;
	else if (ms_strlen(str) == 6 && !ft_strncmp(str, "export", 6))
		result = TRUE;
	free(str);
	return (result);
}

int	send_env_to_father(char **env, int *fd)
{
	int		i;
	int		nb_strs;
	size_t	len;

	if (close(fd[0]) < 0)
		return (perror_fail("Minishell: close()"));
	nb_strs = tab_strlen(env);
	if (write(fd[1], &nb_strs, sizeof(int)) == -1)
		return (perror_fail("Minishell: write()"));
	i = 0;
	while (env[i])
	{
		len = ms_strlen(env[i]) + 1;
		if (write(fd[1], &len, sizeof(size_t)) == -1)
			return (perror_fail("Minishell: write()"));
		if (write(fd[1], env[i], len) == -1)
			return (perror_fail("Minishell: write()"));
		i++;
	}
	if (close(fd[1]) < 0) // pas de bug ?? 
		return (perror_fail("Minishell: close()")); // sur ok de ca pas de bug ???
	return (SUCCESS);
}