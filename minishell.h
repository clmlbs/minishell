/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:29:41 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/31 16:41:57 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <readline/readline.h> 
# include <readline/history.h>
# include <sys/types.h>
# include <signal.h>
# include <sys/wait.h>

#ifndef MINISHELL_H
# define MINISHELL_H

# define SUCCESS 0
# define FAILURE 1
# define EMPTY 2

# define SON 1
# define DAD 0

# define YES 1
# define NO 0

//=========== TOKENS ============
# define WORD 0
# define BACKSLASH_N 2
# define WHITESPACE 32

# define DOUBLE_QUOTE 34
# define SIMPLE_QUOTE 39

# define FILE_NAME 40
# define KEY_WORD_HERE_DOC 41

# define VAR 36
# define PIPE 124

# define SIMPLE_REDIR_IN 58
# define DOUBLE_REDIR_IN 59
# define REDIR_IN 60

# define REDIR_OUT 62
# define SIMPLE_REDIR_OUT 63
# define DOUBLE_REDIR_OUT 64

//============ COLORS ==============
# define WATERMELON "\x1b[38;2;254;127;156m"
# define ORANGE "\x1b[38;2;255;165;0m"
# define WHITE "\x1b[0m"

typedef struct s_cmd
{
	char			**wd;
	int				*is_str;
	int				fd_infile;
	char			*infile_name;
	int				infile_mode;
	int				fd_outfile;
	char			*outfile_name;
	int				outfile_mode;
	char			*good_path;
	int				position;
}	t_cmd;

typedef struct s_lexer
{
	struct s_lexer	*prev;
	char			c;
	char			*token;
	int				id;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_all
{
	t_lexer	*lexer;

	t_cmd	*cmd;
	char	**all_path;
//	char	**env;
	int		end[2];
	int		nb_cmd;
}	t_all;

t_all	g_all;

//=========== ERRORS ============
//errors.c
void	write_error(char *begin, char *middle, char *end);
void	error_token(t_lexer *lst, int len);
int		ft_perror(char *str);

//=========== FREE ============
void	free_tab_strs(char **str);
void	free_t_cmd(t_cmd *cmd);
void	free_t_lexer(t_lexer *lst);

//=========== INIT ============
int		init_t_cmd(t_cmd *cmd);

//=========== LEXER ============
//lexer.c
void	lexer(char *input);
int		tokenize_all_steps(void);

//t_lexer_utils.c
int		add_next_char_to_current(t_lexer *current);
int		add_char_to_str(t_lexer *str_dest, t_lexer *char_src);
void	remove_token(t_lexer *lst);

//token.c
int		init_id(char c);
int		assign_id_to_char(char c, int id);
int		tokenize_words(int id_target, int id_compare);
int		tokenize_quotes(int even, int id);
void	remove_spaces(void);

//token1.c
int		token_redir_or_pipe(char *input, int *index);
int		add_lst_for_quotes(char *input, int begin, int length, int id);
int		token_simple_quote(char *input, int *index, int i, int id);
int		token_double_quote(char *input, int *index, int i, int id);

//token2.c
void	is_token_a_file(t_lexer *lst);
void	token_is_unique(t_lexer *lst);
int		token_is_double(t_lexer *lst);
int		formate_redir_or_pipe(t_lexer *lst);

//=========== LIBFT ============
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *s1);
char	*ms_strjoin(char const *s1, char const *s2);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strzero(char *s, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);

//=========== UTILITIES ============
//lst_utils.c
void	ms_lstclear(t_lexer **lst);
void	ms_lstdelone(t_lexer *lst);
void	lex_lstadd_back(t_lexer **lst, t_lexer *new);
t_lexer	*ft_lstlast(t_lexer *lst);
void	print_t_lexer(void);
t_lexer	*lex_lstnew(void);

//utilities.c
int		isnt_specific(char c);
int		is_var(char c);
int		is_redir_or_pipe(char c);
int 	is_quote(char c);

void	printf_strs(char **strs);
int		copy_tab_of_strs(char **old, char **new);
int		tab_strlen(char **tab_of_str);

void	env_check(char **env);
void	lex_lstadd_back(t_lexer **lst, t_lexer *new);
void	env_to_lst(char **env);
void	env_pwd_update(void);
void	built_env(void);
// void    parser(char *input);

t_lexer	*add_path_and_name_env(char *path, char *name);
t_lexer	*add_token(char *token, int id);
t_lexer	*add_other(char *oth);
t_lexer	*lst_new_env(char *str);
t_lexer	*ft_lst(void);
t_lexer	*add_var_env(char *token);

int		ft_strcmp(const char *s1, const char *s2);

char	**ft_split(const char *s, char c);

//=========== PARSER ============
// parser.c
int		parser(void);

//quotes.c
int		check_quotes_even(t_lexer *lst);
int		remove_quotes(t_lexer *lst);
int		are_quotes_even(void);

//parsermauvais.c
int		update_wd(t_cmd *cmd, t_lexer *lst);
int		fill_cmd(t_cmd *cmd, t_lexer *lst);

//=========== MAIN =============

int		check_line(char *input);
void	minishell(char *input);
int		main(int arc, char **arv, char **env);
#endif
