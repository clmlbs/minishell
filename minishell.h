/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:29:41 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/06 10:03:45 by cleblais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <readline/readline.h> 
#include <readline/history.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>

#ifndef MINISHELL_H
# define MINISHELL_H

# define SUCCESS 0
# define FAILURE 1
# define EMPTY 2

# define SON 1
# define DAD 0

# define YES 1
# define NO 0

# define TRUE 1
# define FALSE 0

# define WITH_INDEX 1
# define WITHOUT_INDEX 0

//=========== TOKENS ============
# define WORD 0
# define BACKSLASH_N 2
# define WHITESPACE 32

# define DOUBLE_QUOTE 34
# define SIMPLE_QUOTE 39

# define KEY_WORD_HERE_DOC 40
# define INFILE_NAME 41
# define OUTFILE_NAME 42

# define VAR 36
# define PIPE 124

# define SIMPLE_REDIR_IN 58
# define DOUBLE_REDIR_IN 59
# define REDIR_IN 60

# define REDIR_OUT 62
# define SIMPLE_REDIR_OUT 63
# define DOUBLE_REDIR_OUT 64

//============ COMMANDS ==============

# define CLASSIC 0
# define READ 1
# define HERE_DOC 2
# define REPLACE 3
# define APPEND 4

//============ COLORS ==============
# define WATERMELON "\x1b[38;2;254;127;156m"
# define ORANGE "\x1b[38;2;255;165;0m"
# define WHITE "\x1b[0m"

typedef struct s_cmd
{
	struct s_cmd	*prev;
	char			**wd;
	int				fd_infile;
	char			*infile_name;
	int				infile_mode;
	int				fd_outfile;
	char			*outfile_name;
	int				outfile_mode;
	char			*good_path;
	int				position;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_lexer
{
	struct s_lexer	*prev;
	char			c;
	char			*token;
	int				id;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_count
{
	unsigned int	count;
	unsigned int	j;
	unsigned int	start;
	size_t			len;
}	t_count;

typedef struct s_doc
{
	char	*input;
	char	*line;
	char	*buf_line;
	int		line_len;
	int		input_len;
}	t_doc;

typedef struct s_all
{
	t_lexer	*lexer;
	t_cmd	*cmd;
	char	**all_path;
	char	**env;
	int		nb_cmd;
	int		*pid;
	int		fd_stdin;
	int		fd_stdout;
	int		end[2];
	int		heritage[2];
	int		size[2];
}	t_all;

t_all	g_all;

//=========== BUILTIN ============
//builtin_1.c
int		is_builtin(t_cmd *cmd);
void	execute_env(t_cmd *cmd);
void	execute_pwd(t_cmd *cmd);
void	execute_echo(t_cmd *cmd);

//unset.c
void	execute_unset(t_cmd	*cmd);
int		send_env_to_father(void);
char	**remove_var(char **env, int index, int i);
char	**error_strdup(char **env, char **new);

//=========== COMMANDS ============
//commands.c
int		add_token_to_cmd(t_lexer *lexer, t_cmd *cmd);
int		fill_t_cmd(void);

//here_doc.c
int		add_key_word_here_doc(t_lexer *lexer, t_cmd *cmd);
char	*here_doc(char *keyword, t_doc *doc);
void	create_here_doc_line(t_doc *doc);
int		add_here_doc_to_cmd(t_lexer *lexer, t_cmd *cmd);

//add_cmds.c
int		add_word(t_lexer *lexer, t_cmd *cmd);
int		add_infile_name(t_lexer *lexer, t_cmd *cmd);
int		add_outfile_name(t_lexer *lexer, t_cmd *cmd);

//=========== ERRORS ============
//errors.c
void	write_error(char *begin, char *middle, char *end);
void	error_token(t_lexer *lst, int len);
int		perror_fail(char *str);
void	perror_void(char *str);

//=========== EXECUTE ============
//execution.c 
int		ft_fork(t_cmd *cmd);
int		execute(t_cmd *cmd_in_global);
void	execute_child(t_cmd *cmd);
void	execute_builtin(t_cmd *cmd);

//execute_utils.c
int		find_good_path(t_cmd *cmd);
int		check_if_openable(t_cmd *cmd);
int		check_if_writable(t_cmd *cmd);
int		dup_fd(t_cmd *cmd);
int		put_size_to_zero(void);


//=========== FREE ============
//free.c
void	free_tab_strs(char **str);
void	free_t_cmd(t_cmd *cmd);
void	free_t_lexer(t_lexer *lst);
void	free_all_lexer(void);
void	free_all_cmd(void);
void	free_all_lexer_and_cmd(void);

//=========== INIT ============
void	init_global(int ac, char **av, char **env);
int		init_env(char **envp);
int		update_global(void);
int		ret_upt(char **new_env, int *nb_strs, int return_value);

//=========== LEXER ============
//lexer.c
int		lexer(char *input);
int		is_var_exist(char *var, int *index);
char	*ft_getvar(char *var);
int		tokenize_all_steps(void);

//t_lexer_utils.c
int		add_next_char_to_current(t_lexer *current);
int		add_char_to_str(t_lexer *str_dest, t_lexer *char_src);
char	*ft_add_char_to_str(char *str, char c);

//token.c
int		init_id(char c);
int		assign_id_to_char(char c, int id);
int		tokenize_words(int id_target, int id_compare);
int		tokenize_quotes(int even, int id);
void	remove_spaces(void);

//var.c
void	update_id_var(void);
int		replace_var(void);
int		update_token(t_lexer *lexer, int *index);

//=========== LIBFT ============
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
char	*ms_strjoin(char const *s1, char const *s2);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strzero(char *s, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**ms_split(char *s, char c);

//=========== T_CMD ===============
//t_cmd_1.c
t_cmd	*cmd_lstlast(t_cmd *lst);
void	cmd_lstadd_back(t_cmd **lst, t_cmd *new);
t_cmd	*cmd_lstnew(void);
int		update_strs(char ***strs, char *to_add);

//t_cmd_2.c
t_cmd	*copy_t_cmd(t_cmd *src);
int		copy_str_t_cmd(t_cmd *new, t_cmd *src);

//=========== UTILITIES ============
//lst_utils.c
void	lex_lstadd_back(t_lexer **lst, t_lexer *new);
t_lexer	*lex_lstlast(t_lexer *lst);
void	print_t_lexer(void);
t_lexer	*lex_lstnew(void);

//utilities.c
int		isnt_specific(char c);
int		is_var(char c);
int		is_redir_or_pipe(char c);
int		is_quote(char c);
int		is_space(char c);
int		is_spe_or_num(char c);
int		ft_isalnum(int c);

void	printf_strs(char **strs, int mode, int fd);
void	print_t_cmd(t_cmd *cmd);
char	**copy_strs_plus_one(char **old);
int		tab_strlen(char **tab_of_str);

void	lex_lstadd_back(t_lexer **lst, t_lexer *new);

t_lexer	*add_path_and_name_env(char *path, char *name);
t_lexer	*add_token(char *token, int id);
t_lexer	*add_other(char *oth);
t_lexer	*lst_new_env(char *str);
t_lexer	*add_var_env(char *token);

//=========== PARSER ============
// parser.c
int		check_pipes(void);
int		check_redir(void);
int		parse_token_after_redir(void);
int		parser(void);

//quotes.c
int		check_quotes_even(t_lexer *lst);
int		remove_quotes(t_lexer *lst);
int		are_quotes_even(void);

//parser_utils.c
void	remove_token(t_lexer *lst);
int		add_next_str_to_current(t_lexer *current);
int		parse_same_id(int id_target);
void	change_id_redir(t_lexer *lst);

//=========== MAIN =============

int		check_line(char *input);
void	ft_waitpid(void);
void	minishell(char *input);
int		main(int arc, char **arv, char **env);
#endif
