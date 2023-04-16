/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 18:22:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 18:25:06 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_here_doc_to_cmd(t_lexer *lexer, t_cmd *cmd)
{
	int	fd[2];

	if (pipe(fd) == -1)
		perror_fail("Minishell");
	if (write(fd[1], lexer->token, ms_strlen(lexer->token)) == -1)
		perror_fail("Minishell");
	if (write(fd[1], "\n", 1) == -1)
		perror_fail("Minishell");
	close(fd[1]);
	cmd->infile_mode = HERE_DOC;
	cmd->fd_infile = fd[0];
	return (SUCCESS);
}

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
	free(lexer->token);
	lexer->token = input;
	free(doc);
	if (add_here_doc_to_cmd(lexer, cmd) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
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
