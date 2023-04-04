/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 08:44:20 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/04 09:52:17 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(t_cmd *cmd)
{
	// if (ft_strlen(cmd->wd[0]) == 2 && !ft_strncmp(cmd->wd[0], "cd", 2))
	// 	return (TRUE);
	/*else*/ if (ft_strlen(cmd->wd[0]) == 3 && !ft_strncmp(cmd->wd[0], "pwd", 3))
		return (TRUE);
	else if (ft_strlen(cmd->wd[0]) == 3 && !ft_strncmp(cmd->wd[0], "env", 3))
		return (TRUE);
	// else if (ft_strlen(cmd->wd[0]) == 4 && !ft_strncmp(cmd->wd[0], "echo", 4))
	// 	return (TRUE);
	// else if (ft_strlen(cmd->wd[0]) == 4 && !ft_strncmp(cmd->wd[0], "exit", 4))
	// 	return (TRUE);
	// else if (ft_strlen(cmd->wd[0]) == 5 && !ft_strncmp(cmd->wd[0], "unset", 5))
	// 	return (TRUE);
	// else if (ft_strlen(cmd->wd[0]) == 6 && !ft_strncmp(cmd->wd[0], "export", 6))
	// 	return (TRUE);
	else
		return (FALSE);
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

// void	execute_echo(t_cmd *cmd)
// {
// 	int	i;

// 	if (cmd->wd[1] == NULL)
// 		ft_putstr_fd("\n", cmd->fd_outfile);
// 	else if (ft_strlen(cmd->wd[1]) == 2 && !ft_strncmp("-n", cmd->wd[1], 2))
// 	{
// 		i = 1;
// 		if (cmd->wd[2] == NULL)
// 			exit(SUCCESS) ;// ca ok ou '\0' ?
// 		while (cmd->wd[++i + 1])
// 		{
// 			ft_putstr_fd(cmd->wd[i], cmd->fd_outfile);
// 			ft_putstr_fd(" ", cmd->fd_outfile);
// 		}
// 		ft_putstr_fd(cmd->wd[i], cmd->fd_outfile);
// 	}
// 	else
// 	{
// 		i = 0;
// 		while (cmd->wd[++i + 1])
// 			printf("%s ", lst->wd[i]);
// 		printf("%s\n", lst->wd[i]);
// 	}
// 	exit(SUCCESS);
// }

void	execute_echo(t_cmd *cmd)
{
	int	i;

	if (cmd->wd[1] == NULL)
		ft_putstr_fd("\n", cmd->fd_outfile);
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