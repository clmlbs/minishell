/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:45:50 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 18:01:03 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_word(t_lexer *lexer, t_cmd *cmd)
{
	if (!cmd->wd)
		cmd->position = g_all.nb_cmd;
	if (update_wd(lexer->token, cmd) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	add_key_word_here_doc(t_lexer *lexer)
{
	char	*input;
	t_doc	*doc;

	doc = (t_doc *)malloc(sizeof(t_doc));
	if (!doc)
		ft_perror("Minishell: malloc()");
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
	lexer->id = WORD;
	free(lexer->token);
	lexer->token = input;
	free(doc);//normalement pas besoin de free chaque truc car deja fait 
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
			perror("Minishell: malloc()");
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