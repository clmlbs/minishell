/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:38:45 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/04 14:15:21 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//extern t_context	g_all;

int	lexer(char *input)
{
	int	i;

	i = 0;
	g_all.lexer = lex_lstnew();
	if (!g_all.lexer)
		return (FAILURE);
	//printf("input:%s\n", input);//*****
	while (input[i])
	{
		if (init_id(input[i]) == FAILURE)
		{
			write_error("Minishell: ", "syntax error", "\n");
			return (FAILURE);
		}
		i++;
	}
	if (tokenize_all_steps() == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

void	update_id_var(void)
{
	t_lexer *buf;

	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == DOUBLE_QUOTE && ft_strchr(buf->token, '$'))
			buf->id = VAR;
		buf = buf->next;
	}
}

int	replace_var(void)
{
	t_lexer	*buf;
	int		i;

	i = 0;
	buf = g_all.lexer;
	while (buf)
	{
		if (buf->id == VAR)
		{
			while ((buf->token[i] && (buf->token)[i] != '$'))
			{
				while ((buf->token[i] && (buf->token)[i] != '$'))
					i++;
				if (update_token(buf, &i) == FAILURE)
					return (FAILURE);
				buf->id = WORD;
			}
		}
		buf = buf->next;
	}
	return (SUCCESS);
}

int	update_token(t_lexer *lexer, int *index)
{
	char	*new;
	char	*var;
	int		i;
	int		begin;
	char 	*fullvar;
	char	*old_and_fullvar;

	new = (char *)malloc(sizeof(char));
	if (!new)
	{
		perror("Minishell");
		return (FAILURE);
	}
	new[0] = '\0';
	i = 0;
	while (i < (*index))
	{
		if (ft_add_char_to_str(new, lexer->token[i]) == FAILURE)
		{
			if (new)
				free(new);
			return (FAILURE);
		}
		i++;
	}
	// Pour passer apres le $
	i++; 
	begin = i;
	// Si le $ est en dernier ou suivi d'un espace on le laisse
	if (!lexer->token[i] || is_space(lexer->token[i]) == TRUE)
	{
		free(new);
		(*index)++;
		return (FAILURE);
	}
	if (is_spe_or_num(lexer->token[i]) == TRUE)
		i++;
	else
	{
		i++;
		while (lexer->token[i] && ft_isalnum(lexer->token[i]) == TRUE)
			i++;
	}
	var = ft_substr(lexer->token, begin, i - begin);
	if (!var)
	{
		free(new);
		return (FAILURE);
	}
	fullvar = ft_getvar(var); // doit renvoyer soit str complete
							// soit NULL si echoue soit '\0' si n'existe pas	
	free(var);
	if (!fullvar)
	{
		free(new);
		return (FAILURE);
	}
	old_and_fullvar = ms_strjoin(new, fullvar);
	free(new);
	free(fullvar);
	if (!old_and_fullvar)
		return (FAILURE);
	(*index) = ft_strlen(old_and_fullvar);
	new = ms_strjoin(old_and_fullvar, \
		ft_substr(lexer->token, i, ft_strlen(lexer->token) - i));
	free(old_and_fullvar);
	if (!new)
		return (FAILURE);
	free(lexer->token);
	lexer->token = new;
	return (SUCCESS);
}

char	*ft_getvar(char *var)
{
	int		i;
	int		exist;
	char	*new;

	exist = NO;
	i = 0;
	while (g_all.env[i])
	{
		if (!ft_strncmp(g_all.env[i], var, ft_strlen(var)))
		{
			exist = YES;
			break ;
		}
		i++;
	}
	if (exist == NO)
	{
		new = (char *)malloc(sizeof(char));
		if (!new)
		{
			perror("Minishell");
			return (NULL);
		}
		new[0] = '\0';
		return (new);
	}
	new = ft_strdup(g_all.env[i] + ft_strlen(var) + 1);
	if (!new)
		return (NULL);
	return (new);
}

int	tokenize_all_steps(void)
{
	if (tokenize_quotes(NO, 0) == FAILURE) // verif si ok
		return (FAILURE);
	if (tokenize_words(VAR, WORD) == FAILURE)
		return (FAILURE);
	update_id_var();
	if (replace_var() == FAILURE)
		return (FAILURE);
	print_t_lexer();
	if (tokenize_words(WORD, WORD) == FAILURE)
		return (FAILURE);
	if (tokenize_words(PIPE, PIPE) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_IN, REDIR_IN) == FAILURE)
		return (FAILURE);
	if (tokenize_words(REDIR_OUT, REDIR_OUT) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}