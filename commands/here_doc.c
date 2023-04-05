/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:03:11 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/05 14:13:22 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_key_word_here_doc(t_lexer *lexer, t_cmd *cmd)
{
	char	*input;
	t_doc	*doc;

	doc = (t_doc *)malloc(sizeof(t_doc));
	if (!doc)
		perror_failure("Minishell: malloc()");
	doc->input = NULL;
	doc->line = NULL;
	doc->buf_line = NULL;
	doc->line_len = 0;
	doc->input_len = 0;
	input = here_doc(lexer->token, doc);
	if (!input)
	{
		free (doc);
		return (FAILURE);
	}
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
	while (1)
	{
		doc->input = readline("> ");
		if (doc->input == NULL || !ft_strncmp(doc->input, keyword, \
			ft_strlen(keyword)))
		{
			free(doc->input);
			break ;
		}
	//	add_history(input); // Je l'ai enlever pour pas fausser l'historique 
		doc->input_len = ft_strlen(doc->input) + 1;
		doc->buf_line = (char *)malloc(sizeof(char) * (doc->line_len + doc->input_len + 1));
		if (!(doc->buf_line))
		{
			perror_void("Minishell: malloc()");
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
}

int	add_here_doc_to_cmd(t_lexer *lexer, t_cmd *cmd)
{
	int	fd[2];

	if (pipe(fd) == -1)
		perror_failure("Minishell");
	if (write(fd[1], lexer->token, ft_strlen(lexer->token)) == -1)
		perror_failure("Minishell");
	if (write(fd[1], "\n", 1) == -1)
		perror_failure("Minishell");
	close(fd[1]); // ici c'est ok ? 
	cmd->infile_mode = HERE_DOC;
	cmd->fd_infile = fd[0];
	return (SUCCESS);
}