/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:09:58 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/06 16:52:36 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	put_in_alphabetic_order(char **strs)
{
	int		i;
	int		j;	
	int		len;
	char	*temp;

	len = tab_strlen(strs);
	i = 0;
	j = 0;
	while (i < len - 1)
	{
		while (j < len - i - 1)
		{
			if (ft_strcmp(strs[j], strs[j + 1]) > 0)
			{
				temp = strs[j];
				strs[j] = strs[j + 1];
				strs[j + 1] = temp;
			}
			j++;
		}
		j = 0;
		i++;
	}
}

void	print_env_in_alphabetic(char **strs)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < tab_strlen(strs))
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		while (strs[i][j] != '=')
		{
			ft_putchar_fd(strs[i][j], STDOUT_FILENO);
			j++;
		}
		ft_putchar_fd(strs[i][j], STDOUT_FILENO);
		ft_putchar_fd('\"', STDOUT_FILENO);
		while (strs[i][++j])
			ft_putchar_fd(strs[i][j], STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
		j = 0;
		i++;
	}
}

void	export_without_args(void)
{
	char	**env_copy;

	env_copy = copy_strs_plus_one(g_all.env);
	if (!env_copy)
		exit(FAILURE);
	put_in_alphabetic_order(env_copy);
	print_env_in_alphabetic(env_copy);
	free_tab_strs(env_copy);
	exit(SUCCESS);
}

void	execute_export(t_cmd *cmd)
{
	if (!cmd->wd[1])
		export_without_args();
}