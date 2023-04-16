/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 09:19:19 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 19:13:32 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	replace_dollar_question_mark(char **strs)
{
	int		i;
	char	*status;

	i = 0;
	status = ft_itoa(g_all.status);
	if (!status)
		return (FAILURE);
	while (strs && strs[i])
	{
		if (ms_strlen(strs[i]) == 2 && !ft_strncmp(strs[i], "$?", 2))
		{
			free(strs[i]);
			strs[i] = ft_strdup(status);
			if (!strs[i])
			{
				free(status);
				return (FAILURE);
			}
		}
		i++;
	}
	free(status);
	return (SUCCESS);
}

int	create_executable_name(t_cmd *cmd)
{
	int		total_len;
	char	*name;

	total_len = ms_strlen(cmd->wd[0]);
	while (total_len >= 0 && cmd->wd[0][total_len - 1] != '/')
		total_len--;
	name = ft_strdup(&(cmd->wd[0][total_len]));
	if (!name)
		return (FAILURE);
	free(cmd->wd[0]);
	cmd->wd[0] = name;
	return (SUCCESS);
}

void	exit_son(int return_value)
{
	close(g_all.herit[0]);
	close(g_all.herit[1]);
	exit(return_value);
}

void	cmd_null(t_cmd *cmd)
{
	int		cmd_nb;
	t_lexer	*buf;

	cmd_nb = 1;
	buf = g_all.lexer;
	while (buf && cmd_nb != cmd->position)
	{	
		if (buf->id == PIPE)
			cmd_nb++;
		buf = buf->next;
	}
	if (buf && buf->c == '$')
		exit(0);
	if (buf && (buf->c == '\'' || buf->c == '\"'))
	{
		write_error("Minishell: : command not ", "found", "\n");
		exit(127);
	}
	else
		exit(1);
}
