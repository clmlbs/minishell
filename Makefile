# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 08:01:18 by cleblais          #+#    #+#              #
#    Updated: 2023/04/04 09:19:26 by cleblais         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

SRCS		=	main.c \
				builtin/builtin_1.c \
				commands/commands.c \
				commands/here_doc.c \
				commands/add_cmds.c \
				errors/errors.c \
				execution/execution.c \
				execution/execute_utils.c \
				free/free.c \
				init/init.c \
				lexer/env.c \
				lexer/lexer.c \
				lexer/t_lexer_utils.c \
				lexer/token.c \
				libft/ft_putstr_fd.c \
				libft/ft_split.c \
				libft/ft_strdup.c \
				libft/ft_strlcat.c \
				libft/ft_strlcpy.c \
				libft/ft_strlen.c \
				libft/ft_strncmp.c \
				libft/ft_strtrim.c \
				libft/ft_strzero.c \
				libft/ft_substr.c \
				libft/ms_strjoin.c \
				libft/ms_split.c \
				parser/parser_utils.c \
				parser/parser.c \
				parser/quotes.c \
				t_cmd/t_cmd_1.c \
				t_cmd/t_cmd_2.c \
				utilities/stack_function.c \
				utilities/utilities.c \
				utilities/utilities_second.c \
				utilities/lst_utils.c
				

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