/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:03:11 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 18:46:47 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*here_doc(char *keyword, t_doc *doc)
{
	pid_t	pid;
	int		end[2];

	g_all.where = HERE_DOC;
	if (pipe(end) == -1)
	{
		perror("Minishell: pipe()");
		return (NULL);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("Minishell: fork()");
		g_all.where = PROCESS;
		return (NULL);
	}
	else if (pid == 0)
		here_doc_son(keyword, doc, end[1], end[0]);
	else
	{
		g_all.my_pid = pid;
		return (here_doc_father(doc, end[1], end[0]));
	}
	return (NULL);
}

char	*here_doc_father(t_doc *doc, int end_1, int end_0)
{
	char	*line;
	size_t	len;

	len = 0;
	line = NULL;
	close(end_1);
	read(end_0, &len, sizeof(size_t));
	g_all.where = PROCESS;
	if (len == 0)
	{
		close(end_0);
		return (NULL);
	}
	else
		line = (char *)malloc(len);
	if (!line)
	{
		close(end_0);
		perror("Minishell: malloc()");
		return (NULL);
	}
	read(end_0, line, len);
	close(end_0);
	return (line);
}

void	here_doc_son(char *keyword, t_doc *doc, int end_1, int end_0)
{
	char	*line;
	size_t	len;

	len = 0;
	line = NULL;
	g_all.my_pid = 0;
	close(end_0);
	line = readline_here_doc(keyword, doc);
	g_all.where = PROCESS;
	if (!line)
	{	
		line = (char *)malloc(sizeof(char));
		line[0] = '\0';
	}
	len = ms_strlen(line) + 1;
	if (write(end_1, &len, sizeof(size_t)) == -1)
		perror("Minishell: write()");
	else if (write(end_1, line, len) == -1)
		perror("Minishell: write()");
	exit(SUCCESS);
}

char	*readline_here_doc(char *keyword, t_doc *doc)
{
	while (1)
	{
		doc->input = readline("> ");
		if (doc->input == NULL || (!ft_strncmp(doc->input, keyword, \
			ms_strlen(keyword))))
		{
			free(doc->input);
			break ;
		}
		doc->input_len = ms_strlen(doc->input) + 1;
		doc->buf_line = (char *)malloc(sizeof(char) * \
			(doc->line_len + doc->input_len + 1));
		if (!(doc->buf_line))
		{
			perror_void("Minishell: malloc()");
			g_all.where = PROCESS;
			if (doc->input)
				free(doc->input);
			return (NULL);
		}
		create_here_doc_line(doc);
	}
	if (doc->line)
		return (finish_doc_line(doc));
	return (NULL);
}

char	*finish_doc_line(t_doc *doc)
{
	doc->line[doc->line_len - 1] = '\0';
	doc->line[doc->line_len] = '\0';
	if (doc->input == NULL)
		printf("\033[A> ");
	return (doc->line);
}
