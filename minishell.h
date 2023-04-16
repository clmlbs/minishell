/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:29:41 by cleblais          #+#    #+#             */
/*   Updated: 2023/04/16 17:35:39 by cleblais         ###   ########.fr       */
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
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "libft.h"

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
# define CHAR_SPE 1
# define BACKSLASH_N 2
# define WHITESPACE 32

# define DOUBLE_QUOTE 34
# define SIMPLE_QUOTE 39

# define KEY_WORD_HERE_DOC 40
# define INFILE_NAME 41
# define OUTFILE_NAME 42

# define VAR 36
# define PIPE 124

# define REDIR 70
# define SUPPR 80

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

# define PROCESS 0
# define EXECUTION 3

//============ COLORS ==============
# define WATERMELON "\x1b[38;2;254;127;156m"
# define ORANGE "\x1b[38;2;255;165;0m"
# define WHITE "\x1b[0m"

//============ STRUCTURES ================
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
	char			*save;
	int				id;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_doc
{
	char	*input;
	char	*line;
	char	*buf_line;
	int		line_len;
	int		input_len;
}	t_doc;

typedef struct s_update_token
{
	int		i;
	int		begin;
	char	*str_begin;
	char	*var_name;
	char 	*var_value;
	char	*begin_and_value;
	char	*token_updated;
}	t_update_token;

typedef struct s_all
{
	t_lexer	*lexer;
	t_cmd	*cmd;
	char	**all_path;
	char	**env;
	char	*tilde;
	int		nb_cmd;
	int		*pid; // utile ??
	pid_t	my_pid;
	int		fd_stdin;
	int		fd_stdout;
	int		end[2];
	int		herit[2];
	int		is_first_turn;
	int		status;
	int		where;
}	t_all;

t_all	g_all;

//=========== BUILTIN ============
//builtin_utils.c
char	*put_to_lower(char *str);
void	change_var_value(char *str, int *env_index);
void	put_in_alphabetic_order(char **strs);

//builtin.c
int		is_builtin(t_cmd *cmd);
int		send_env_to_father(char **env, int *fd);
size_t	var_name_len(char *str);

//cd_1.c
int		add_var_to_env(char *str);
int		add_or_update_var_in_env(char *var_str);
int		add_oldpwd(t_cmd *cmd);
int		replace_path(t_cmd *cmd);
void	execute_cd(t_cmd *cmd);

//cd_2.c
int		replace_tilde(t_cmd *cmd);
int		replace_home(t_cmd *cmd);
int		replace_oldpwd(t_cmd *cmd);
int		check_destination(t_cmd *cmd);
int		add_newpwd(t_cmd *cmd);

//echo.c
void	echo_contain_n(t_cmd *cmd, int *i);
void	execute_echo(t_cmd *cmd, int i);

//env.c
void	execute_env(t_cmd *cmd);

//exit.c
char	*remove_spaces_tab(char *str);
int		check_number(char *str, t_cmd *cmd);
void	exit_negatif(long long code);
void	execute_exit(t_cmd *cmd);

//export.c
void	export_without_args(void);
void	export_var(char *str);
int		export_check_args(char **strs, int *i);
void	execute_export(t_cmd *cmd, int i, int j);
void	print_env_in_alphabetic(char **strs, int i, int j);

//pwd.c
void	execute_pwd(t_cmd *cmd);

//unset.c
int		unset_var(char ***new, char *var, int *index);
int		unset_check_args(char **strs, int *var_target);
void	execute_unset(t_cmd	*cmd);
char	**remove_var(char **env, int index, int i);
char	**error_strdup(char **env, char **new);

//=========== COMMANDS ============
//commands.c
int		add_token_to_cmd(t_lexer *lexer, t_cmd *cmd);
int		fill_t_cmd(void);

//here_doc.c
int		add_key_word_here_doc(t_lexer *lexer, t_cmd *cmd);
char	*here_doc(char *keyword, t_doc *doc);
char	*here_doc_son(char *keyword, t_doc *doc);
void	create_here_doc_line(t_doc *doc);
int		add_here_doc_to_cmd(t_lexer *lexer, t_cmd *cmd);

//add_cmds.c
int		add_word(t_lexer *lexer, t_cmd *cmd);
int		add_infile_name(t_lexer *lexer, t_cmd *cmd);
int		add_outfile_name(t_lexer *lexer, t_cmd *cmd);

//=========== ERRORS ============
//errors.c
void	write_error(char *begin, char *middle, char *end);
void	error_token(t_lexer *lst, int len, int token_id);
int		perror_fail(char *str);

//=========== EXECUTE ============
//execution.c 
int		execute_line(void);
int		ft_fork(t_cmd *cmd);
int		execute(t_cmd *cmd_in_global);
void	execute_child(t_cmd *cmd);
void	exit_son(int return_value);
void	execute_builtin(t_cmd *cmd);

//execute_utils.c
int		replace_dollar_question_mark(char **strs);
int		find_good_path(t_cmd *cmd);
int		check_if_openable(t_cmd *cmd);
int		check_if_writable(t_cmd *cmd);
int		dup_fd(t_cmd *cmd);

//=========== FREE ============
//free.c
void	free_t_cmd(t_cmd *cmd);
void	free_one_lst_lexer(t_lexer *lst);
void	free_all_lexer(void);
void	free_all_cmd(void);
void	free_all_lexer_and_cmd(void);
void	free_everything(void);

//=========== INIT ============
void	init_global(int ac, char **av, char **env);
int		save_all_path(char **envp);
int		update_global(void);
int		ret_upt(char **new_env, int *nb_strs, int return_value);
void	init_t_updated_token(t_update_token *t);

//=========== LEXER ============
//lexer.c
int		lexer(char *input);
int		is_var_exist(char *var, int *index);
char	*create_var_value(char *var);
int		tokenize_exit_status_var(void);
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

//var_utils.c
void	update_id_var(void);
int		add_until_dollar(char **str, int *i, int index, t_lexer *lexer);
int		end_of_dollar(char **str, t_lexer *lexer, int *index);
int		end_of_dollar_in_quotes(char **str, t_lexer *lexer, int *index, int *i);
void	go_until_end_of_var(t_lexer *lexer, int *i);

//var.c
int		replace_var(void);
int		create_var_name(t_lexer *lexer, t_update_token *t);
int		create_token_updated(t_lexer *lexer, t_update_token *t, int *index);
int		update_token(t_lexer *lexer, int *index);

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
int		is_special_char(char c);
int		is_special_char_or_numeric(char c);
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
int		check_pipes_begin(void);
int		nb_redir(int id_target);
int		parse_redir(void);
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

//=========== SIGNAL ===========
//signal.c
void	signal_sigint(void);
void	signal_sigquit(void);
void	signal_handle(int signal);
void	echo_ctl(int n);

//=========== MAIN =============

int		check_line(char *input);
void	ft_waitpid(void);
void	minishell(char *input);
int		main(int arc, char **arv, char **env);
#endif
