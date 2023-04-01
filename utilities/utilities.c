/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:05:21 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 16:44:50 by cleblais         ###   ########.fr       */
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

int	is_var(char c)
{
	if (c == '$')
		return (1);
	return (0);
}

void	printf_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		printf("%d] %s\n", i, strs[i]);
		i++;
	}
}

void	print_t_cmd(void)
{
	t_cmd	*buf;

	buf = g_all.cmd;
	while (buf)
	{
		printf_strs(buf->wd);
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

int	copy_tab_of_strs(char **old, char **new)
{
	int tab_len;
	int	i;

	if (!old)
		return (SUCCESS); //ok bien protege ca ? 
	tab_len = tab_strlen(old);
	i = 0;
	while (i < tab_len)
	{
		new[i] = ft_strdup(old[i]);
		if (!new[i])
		{
			free_tab_strs(new);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
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