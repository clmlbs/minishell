/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 08:44:20 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/09 16:10:57 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_builtin(t_cmd *cmd)
{
	if (ft_strlen(cmd->wd[0]) == 2 && !ft_strncmp(cmd->wd[0], "cd", 2))
		execute_cd(cmd);
	else if (ft_strlen(cmd->wd[0]) == 3 && !ft_strncmp(cmd->wd[0], "pwd", 3))
		execute_pwd(cmd);
	else if (ft_strlen(cmd->wd[0]) == 3 && !ft_strncmp(cmd->wd[0], "env", 3))
		execute_env(cmd);
	else if (ft_strlen(cmd->wd[0]) == 4 && !ft_strncmp(cmd->wd[0], "echo", 4))
		execute_echo(cmd);
	else if (ft_strlen(cmd->wd[0]) == 5 && !ft_strncmp(cmd->wd[0], "unset", 5))
		execute_unset(cmd);
	else if (ft_strlen(cmd->wd[0]) == 6 && !ft_strncmp(cmd->wd[0], "export", 6))
		execute_export(cmd);
	else
		exit(FAILURE);
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
		perror(ORANGE "Minishell: getcwd()" WHITE);
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
	else if (ft_strlen(cmd->wd[1]) == 2 && !ft_strncmp("-n", cmd->wd[1], 2))
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
	if (!(ft_strlen(cmd->wd[1]) == 2 && !ft_strncmp("-n", cmd->wd[1], 2)))
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
		len = ft_strlen(env[i]) + 1;
		if (write(fd[1], &len, sizeof(size_t)) == -1)
			return (perror_fail("Minishell: write()"));
		if (write(fd[1], env[i], len) == -1)
			return (perror_fail("Minishell: write()"));
		i++;
	}
	return (SUCCESS);
}