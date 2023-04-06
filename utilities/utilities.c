/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:05:21 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/06 09:14:58 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	isnt_specific(char c)
{
	if (c >= 33 && c <= 126 && !is_redir_or_pipe(c) && !is_var(c) \
		&& !is_quote(c))
		return (SUCCESS);
	return (FAILURE);
}

int	is_var(char c)
{
	if (c == '$')
		return (1);
	return (0);
}

int	is_redir_or_pipe(char c)
{
	if ((c == '<') || (c == '>' || c == '|'))
		return (1);
	return (0);
}

int is_quote(char c)
{
	if (c == 34 || c == 39)
		return (1);
	return (0);
}

int is_space(char c)
{
	if (c == '\n' || c == 9 || c == 11 || c == ' ')
		return (TRUE);
	return (FALSE);
}

int	is_spe_or_num(char c)
{
	if (c >= 33 && c <= 64)
		return (TRUE);
	if ((c >= 91 && c <= 96) && c != '_')
		return (TRUE);
	if (c <= 123 && c >= 127)
		return (TRUE);
	return (FALSE);
}

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
		return (TRUE);
	else
		return (FALSE);
}

void	printf_strs(char **strs, int mode, int fd)
{
	int	i;

	i = 0;
	if (strs)
	{
		if (mode == WITH_INDEX)
		{
			while (strs[i])
			{
				printf("%d] %s\n", i, strs[i]);
				i++;
			}
		}
		else
		{
			while (strs[i])
			{
				ft_putstr_fd(strs[i], fd);
				ft_putstr_fd("\n", fd);
				i++;
			}
		}
	}
}

void	print_t_cmd(t_cmd *cmd)
{
	t_cmd	*buf;

	if (cmd == NULL)
		buf = g_all.cmd;
	else
		buf = cmd;
	while (buf)
	{
		printf_strs(buf->wd, WITH_INDEX, STDOUT_FILENO);
		printf("fd_infile	%d\n", buf->fd_infile);
		printf("fd_outfile	%d\n",  buf->fd_outfile);
		printf("I_mode:		%d\n", buf->infile_mode);
		printf("O_mode:		%d\n", buf->outfile_mode);
		printf("I_name:		%s\n", buf->infile_name);
		printf("O_name: 	%s\n", buf->outfile_name);
		printf("good_path:	%s\n", buf->good_path);
		printf("position:	%d\n\n", buf->position);
		buf = buf->next;
	}
}

char	**copy_strs_plus_one(char **old)
{
	int		tab_len;
	int		i;
	char	**new;

	tab_len = tab_strlen(old);
	new = (char **)malloc(sizeof(char *) * (tab_len + 2));
	if (!new)
		perror_fail("Minishell: malloc()");
	if (!new)
		return (NULL);
	i = -1;
	while (++i < tab_len)
	{
		new[i] = ft_strdup(old[i]);
		if (!new[i])
		{
			free_tab_strs(new);
			return (NULL);
		}
	}
	new[tab_len] = NULL;
	new[tab_len + 1] = NULL;
	return (new);
}

int	tab_strlen(char **tab_of_str)
{
	int i;

	if (tab_of_str == NULL || tab_of_str[0] == NULL)
		return (0);
	i = 0;
	while (tab_of_str[i])
		i++;
	return (i);
}