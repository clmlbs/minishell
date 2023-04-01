/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:45:50 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/01 16:55:57 by cleblais         ###   ########.fr       */
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

	input = here_doc(lexer->token);
	if (!input)
		return (FAILURE);
	lexer->id = WORD;
	free(lexer->token);
	lexer->token = input;
	return (SUCCESS);
}

char *here_doc(char *keyword)
{
	char	*input;
	char	*line;
	char	*buf_line;
	int		line_len;
	int		input_len;

	input = NULL;
	line = NULL;
	line_len = 0;
	while (1)
	{
		input = readline("> ");
		if (input == NULL || !ft_strncmp(input, keyword, ft_strlen(keyword)))
		{
			free(input);
			break ;
		}
	//	add_history(input); // Je l'ai enlever pour pas fausser l'historique 
		input_len = ft_strlen(input) + 1;
		buf_line = (char *)malloc(sizeof(char) * (line_len + input_len + 1));
		buf_line[0] = '\0';
		if (!buf_line)
		{
			perror("Minishell: malloc()");
			return (NULL);
		}
		if (line)
			ft_strlcat(buf_line, line, line_len + 1);
		ft_strlcat(buf_line, input, line_len + input_len + 1);
		line_len += input_len;
		buf_line[line_len - 1] = '\n';
		buf_line[line_len] = '\0';
		free(line);
		line = buf_line;
		free(input);
	}
	if (line)
	{
		line[line_len - 1] = '\0';
		line[line_len] = '\0';
		return (line);
	}
	return (NULL);
}