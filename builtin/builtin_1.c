/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 08:44:20 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/14 17:38:20 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*put_to_lower(char *str)
{
	char	*new;
	int		i;

	i = 0;
	new = ft_strdup(str);
	if (!new)
		return (NULL);
	while (new[i])
	{
		new[i] = ft_tolower(new[i]);
		i++;
	}
	return (new);
}

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

void	execute_env(t_cmd *cmd)
{
	if (cmd->wd[1])
	{
		write_error("Minishell: ", "error: env should be executed ",\
		"without option nor argument\n");
		exit(FAILURE);
	}
	else
	{
		printf_strs(g_all.env, WITHOUT_INDEX, cmd->fd_outfile);
		// mettre le bon status
		exit(SUCCESS);
	}
}

void	execute_pwd(t_cmd *cmd)
{
	char	cwd[1024];

	if (cmd->wd[1] && cmd->wd[1][0] == '-')
	{
		write_error("Minishell: ", "error: pwd should be executed ",\
		"without option\n");
		exit(FAILURE);
	}
	if (!getcwd(cwd, 1024))
	{
		perror("Minishell: getcwd()");
		exit(FAILURE);
	}
	ft_putstr_fd(cwd, cmd->fd_outfile);
	ft_putstr_fd("\n",cmd->fd_outfile);
	exit(SUCCESS);
}

void	execute_echo(t_cmd *cmd)
{
	int	i;

	if (cmd->wd[1] == NULL)
	{
		ft_putstr_fd("\n", cmd->fd_outfile);
		exit(SUCCESS);
	}
	else if (ms_strlen(cmd->wd[1]) == 2 && !ft_strncmp("-n", cmd->wd[1], 2))
	{
		if (cmd->wd[2] == NULL)
			exit(SUCCESS) ;
		i = 1;
	}
	else
		i = 0;
	while (cmd->wd[++i + 1])
	{
		ft_putstr_fd(cmd->wd[i], cmd->fd_outfile);
		ft_putstr_fd(" ", cmd->fd_outfile);
	}
	ft_putstr_fd(cmd->wd[i], cmd->fd_outfile);
	if (!(ms_strlen(cmd->wd[1]) == 2 && !ft_strncmp("-n", cmd->wd[1], 2)))
		ft_putstr_fd("\n", cmd->fd_outfile);
	exit(SUCCESS);
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
	return (SUCCESS);
}