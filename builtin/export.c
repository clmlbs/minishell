/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:09:58 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/10 07:43:31 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	export_var(char *str)
{
	char	**buf;

	buf = copy_strs_plus_one(g_all.env);
	if (!buf)
		exit(perror_fail("Minishell: malloc()"));
	buf[tab_strlen(g_all.env)] = ft_strdup(str);
	if (!buf[tab_strlen(g_all.env)])
		exit(perror_fail("Minishell: malloc()"));
	free_tab_strs(g_all.env);
	g_all.env = buf;
	buf = NULL;
}

int	export_check_args(char **strs, int *i)
{
	int	j;
	int equal_present;

	j = 0;
	equal_present = FAILURE;
	if (strs[*i] && strs[*i][0] == '-')
	{
		write_error("Minishell: ", "options for export are not ", \
			"implemented in this Minishell\n");
		return (FAILURE);
	}
	else if (strs[*i] && is_special_char_or_numeric(strs[*i][0]) == TRUE)
	{
		write_error("Minishell: ", "export:", " `");
		ft_putstr_fd(strs[*i], STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (FAILURE);
	}
	while (strs[*i][j])
	{
		if (strs[*i][j] == '=')
			equal_present = SUCCESS;
		j++;
	}
	return (equal_present);
}

void	execute_export(t_cmd *cmd)
{
	int		i;
	//char	**buf;
	int		j;
	char	*var;

	i = 1;
	j = 0;
	if (!cmd->wd[1])
		export_without_args();
	while (cmd->wd[i])
	{
		if (export_check_args(cmd->wd, &i) == SUCCESS)
		{
			var = ft_substr(cmd->wd[i], 0, var_name_len(cmd->wd[i]));
			if (var && is_var_exist(var, &j) == SUCCESS)
				change_var_value(cmd->wd[i], &j);
			else if (var)
				export_var(cmd->wd[i]);
			j = 0;
		}
		i++;
	}
	if (send_env_to_father(g_all.env, g_all.herit) == FAILURE)
		exit(FAILURE);
	exit(SUCCESS);
}