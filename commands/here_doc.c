/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:03:11 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/11 18:41:11 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_key_word_here_doc(t_lexer *lexer, t_cmd *cmd)
{
	char	*input;
	t_doc	*doc;

	doc = (t_doc *)malloc(sizeof(t_doc));
	if (!doc)
		perror_fail("Minishell: malloc()");
	doc->input = NULL;
	doc->line = NULL;
	doc->buf_line = NULL;
	doc->line_len = 0;
	doc->input_len = 0;
	input = here_doc(lexer->token, doc);
	if (!input || g_all.status == 130)
	{
		free (doc);
		g_all.status = 1;
		return (FAILURE);
	}
	if (input[0] == '\0')
		printf("\033[A\033[A\r> ");
	free(lexer->token);
	lexer->token = input;
	free(doc);//normalement pas besoin de free chaque truc car deja fait 
	if (add_here_doc_to_cmd(lexer, cmd) == FAILURE)
		return (FAILURE);
	// close le fd du here_doc ?? 
	return (SUCCESS);
}

char	*here_doc(char *keyword, t_doc *doc)
{
	pid_t	pid;
	int		end[2];
	char	*line;
	size_t	len;
	
	line = NULL;
	len = 0;
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
		g_all.where = DAD;
		return (NULL);
	}
	else if (pid == 0)
	{
		//======= MARCHE ==========
		// g_all.my_pid = 0;
		// close(end[0]);
		// line = here_doc_son(keyword, doc);
		// g_all.where = DAD;
		// if (!line)
		// 	exit(FAILURE); // OK ca pour le ctl d ? 
		// len = ms_strlen(line) + 1;
		// if (write(end[1], &len, sizeof(size_t)) == -1)
		// 	perror("Minishell: write()");
		// else if (write(end[1], line, len) == -1)
		// 	perror("Minishell: write()");
		// exit(SUCCESS);

		g_all.my_pid = 0;
		close(end[0]);
		line = here_doc_son(keyword, doc);
		g_all.where = DAD;
		if (!line)
		{	
			line = (char *)malloc(sizeof(char));
			line[0] = '\0';
		}
		len = ms_strlen(line) + 1;
		if (write(end[1], &len, sizeof(size_t)) == -1)
			perror("Minishell: write()");
		else if (write(end[1], line, len) == -1)
			perror("Minishell: write()");
		exit(SUCCESS);
	}
	else
	{
		g_all.my_pid = pid;
		len = 0;
		close(end[1]);
		read(end[0], &len, sizeof(size_t));
		g_all.where = DAD;
		if (len == 0)
		{
			close(end[0]);
			return (NULL);
		}
		else
			line = (char *)malloc(len);
		if (!line)
		{
			close(end[0]);
			perror("Minishell: malloc()");
			return (NULL);
		}
		read(end[0], line, len);
		close(end[0]);
		return(line);
	}
}

char	*here_doc_son(char *keyword, t_doc *doc)
{
	while (1)
	{
		doc->input = readline("> ");
		if (doc->input == NULL)
			return (NULL);
		if /*(doc->input == NULL || */(!ft_strncmp(doc->input, keyword, \
			ms_strlen(keyword)))
		{
			free(doc->input);
			break ;
		}
		doc->input_len = ms_strlen(doc->input) + 1;
		doc->buf_line = (char *)malloc(sizeof(char) * (doc->line_len + doc->input_len + 1));
		if (!(doc->buf_line))
		{
			perror_void("Minishell: malloc()");
			g_all.where = DAD;
			return (NULL);
		}
		create_here_doc_line(doc);
	}
	if (doc->line)
	{
		doc->line[doc->line_len - 1] = '\0';
		doc->line[doc->line_len] = '\0';
		return (doc->line);
	}
	return (NULL);
}

void	create_here_doc_line(t_doc *doc)
{
	(doc->buf_line)[0] = '\0';
	if (doc->line)
		ft_strlcat(doc->buf_line, doc->line, doc->line_len + 1);
	ft_strlcat(doc->buf_line, doc->input, doc->line_len + doc->input_len + 1);
	doc->line_len += doc->input_len;
	doc->buf_line[doc->line_len - 1] = '\n';
	doc->buf_line[doc->line_len] = '\0';
	free(doc->line);
	doc->line = doc->buf_line;
	free(doc->input);
	doc->input = NULL;
}

int	add_here_doc_to_cmd(t_lexer *lexer, t_cmd *cmd)
{
	int	fd[2];

	if (pipe(fd) == -1)
		perror_fail("Minishell");
	if (write(fd[1], lexer->token, ms_strlen(lexer->token)) == -1)
		perror_fail("Minishell");
	if (write(fd[1], "\n", 1) == -1)
		perror_fail("Minishell");
	close(fd[1]); // ici c'est ok ? que si statut = 130 ? et return -1 ?
	cmd->infile_mode = HERE_DOC;
	cmd->fd_infile = fd[0];
	return (SUCCESS);
}