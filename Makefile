# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 08:01:18 by cleblais          #+#    #+#              #
#    Updated: 2023/04/16 22:03:54 by cleblais         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

SRCS		=	main.c \
				builtin/builtin.c \
				builtin/builtin_utils.c \
				builtin/cd_1.c \
				builtin/cd_2.c \
				builtin/echo.c \
				builtin/env.c \
				builtin/exit.c \
				builtin/unset.c \
				builtin/export.c \
				builtin/pwd.c \
				commands/commands.c \
				commands/here_doc.c \
				commands/here_doc_utils.c \
				commands/add_cmds.c \
				errors/errors.c \
				execution/check_execute.c \
				execution/execute_utils.c \
				execution/execution.c \
				execution/fork_execute.c \
				free/free.c \
				lexer/lexer_utils.c \
				lexer/lexer.c \
				lexer/id.c \
				lexer/tokenize.c \
				lexer/var_manip_for_lexer.c \
				lexer/tokenize_var.c \
				libft/ft_itoa.c \
				libft/ft_perror.c \
				libft/ft_putnbr_fd.c \
				libft/ft_putstr_fd.c \
				libft/ft_split.c \
				libft/ft_strchr.c \
				libft/ft_strcmp.c \
				libft/ft_strdup.c \
				libft/ft_strlcat.c \
				libft/ft_strlcpy.c \
				libft/ms_strlen.c \
				libft/ft_strncmp.c \
				libft/ft_strtrim.c \
				libft/ft_strzero.c \
				libft/ft_substr.c \
				libft/ft_tolower.c \
				libft/ms_atoi.c \
				libft/ms_strjoin.c \
				libft/ms_split.c \
				parser/parser_checks.c \
				parser/parser_utils.c \
				parser/parser.c \
				parser/quotes.c \
				parser/remover.c \
				set/init_global.c \
				set/set_env.c \
				set/update_env_after_son.c \
				signal/signal.c \
				t_cmd/t_cmd_1.c \
				t_cmd/t_cmd_2.c \
				utilities/utilities.c \
				utilities/lst_utils.c \
				utilities/waitpid.c \
				utilities/var.c
				

HEADERS		=		-I/Users/$(USER)/.brew/opt/readline/include -I include

LIB_PATH	=		-L. -lreadline -L/Users/$(USER)/.brew/opt/readline/lib

OBJS		= 		$(SRCS:.c=.o)

CC			=		gcc

CFLAGS		=		-Wall -Wextra -Werror -g -lreadline -fsanitize=address

RM			= 		rm -f

all			:		$(NAME)

.c.o		:		
					@$(CC) $(FLAGS) $(HEADERS) -c $< -o $(<:.c=.o)

$(NAME)		:		$(OBJS)
					$(CC) $(CFLAGS) $(OBJS) $(HEADERS) $(LIB_PATH) -o $(NAME)

clean		:
					$(RM) $(OBJS)

fclean		:		clean
					$(RM) $(NAME)

re			:		fclean all