/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 08:59:54 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/09 11:05:08 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>

char	*remove_spaces_tab(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || (str[i] > 8 && str[i] < 14)))
		i++;
	return (&(str[i]));
}

int	check_number(char *str, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (FAILURE);
		i++;
	}
	if (!str[0])
		return (FAILURE);
	return (SUCCESS);
}

void	exit_negatif(long long code)
{
	code = code % 256;
	code = 256 + code;
	exit((int)code);
}

void	execute_exit(t_cmd *cmd)
{
	long long	code;
	char		*str;

	str = remove_spaces_tab(cmd->wd[1]);
	if (ft_strlen(str) > 20 || check_number(str, cmd) == FAILURE)
	{
		ft_putstr_fd("exit\nMinishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->wd[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(255); 
	}
	if (cmd->wd[2])
	{
		ft_putstr_fd("exit\nMinishell: exit: too many arguments\n", 2);
		// changer un truc dans la globale pour le statut ? 
		return ;
	}
	code = ms_atoi(str);
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (code < 0)
		exit_negatif(code);
	else if (code > 255)
		exit(code % 256);
	else
		exit((int)code);
}