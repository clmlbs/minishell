/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:29:41 by cleblais          #+#    #+#             */
/*   Updated: 2023/03/30 14:26:36 by cleblais         ###   ########.fr       */
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

# define WORD 0
# define SIMPLE_REDIR_IN 1
# define DOUBLE_REDIR_IN 2
# define SIMPLE_REDIR_OUT 3
# define DOUBLE_REDIR_OUT 4
# define PIPE 5
# define FILE_NAME 6
# define HERE_DOC 7
# define VAR 8

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

typedef struct s_list
{
	struct s_list	*prev;
	char			*token;
	int				id;

	char			*name_env;
	char			*path_env;

	struct s_list	*next;
}	t_list; // changer en t_lexer et pas t_list

typedef struct s_all
{
	t_list	*lexer;
	t_list	*env;

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
void	error_token(t_list *lst, int len);
int		ft_perror(char *str);

//=========== FREE ============
void	free_tab_strs(char **str);
void	free_t_cmd(t_cmd *cmd);

//=========== INIT ============
int		init_t_cmd(t_cmd *cmd);

//=========== LEXER ============
//lexer.c
t_list	*new_lexer(void);
void	lexer(char *input);
int		init_token_id(char *input);

//token.c
void	is_token_a_file(t_list *lst);
void	token_is_unique(t_list *lst);
int		token_is_double(t_list *lst);
int		formate_redir_or_pipe(t_list *lst);
int		formate_token_id(void);

//=========== LIBFT ============
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strtrim(char const *s1, char const *set);

//=========== UTILITIES ============
//lst_utils.c
void	ms_lstclear(t_list **lst);
void	ms_lstdelone(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstlast(t_list *lst);

//utilities.c
int		isnt_specific(char c);
int		is_var(char c);
int		is_redir_or_pipe(char c);
int 	is_quote(char c);

void	printf_strs(char **strs);
int		copy_tab_of_strs(char **old, char **new);
int		tab_strlen(char **tab_of_str);

void	env_check(char **env);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	env_to_lst(char **env);
void	env_pwd_update(void);
void	built_env(void);
// void    parser(char *input);

t_list	*add_path_and_name_env(char *path, char *name);
t_list	*add_token(char *token, int id);
t_list	*add_other(char *oth);
t_list	*lst_new_env(char *str);
t_list	*ft_lst(void);
t_list	*add_var_env(char *token);

int		ft_strcmp(const char *s1, const char *s2);

char	**ft_split(const char *s, char c);

//=========== PARSER ============
//parser.c
void	parser(int i);
int		update_wd(t_cmd *cmd, t_list *lst);
int		fill_cmd(t_cmd *cmd, t_list *lst);

#endif
