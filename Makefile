# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 08:01:18 by cleblais          #+#    #+#              #
#    Updated: 2023/03/30 10:03:16 by cleblais         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=		minishell

SRCS		=	main.c \
				free/free.c \
				init/init.c \
				lexer/env.c \
				lexer/lexer.c \
				lexer/token.c \
				libft/ft_strdup.c \
				libft/ft_strlcpy.c \
				libft/ft_strlen.c \
				parser/parser.c \
				utilities/stack_function.c \
				utilities/utilities.c \
				utilities/ft_split.c \
				utilities/utilities_second.c \
				utilities/lst_utils.c \
				builtin/built_env.c \
				errors/errors.c

HEADERS		=		-I/Users/$(USER)/.brew/opt/readline/include -I include

LIB_PATH	=		-L. -lreadline -L/Users/$(USER)/.brew/opt/readline/lib

OBJS		= 		$(SRCS:.c=.o)

CC			=		gcc

CFLAGS		=		-Wall -Wextra -Werror -g -lreadline

RM			= 		rm -f

all			:		$(NAME)

.c.o		:		
					@$(CC) $(FLAGS) $(HEADERS) -c $< -o $(<:.c=.o)

$(NAME)		:		$(OBJS)
					$(CC) $(CFLAGS) $(OBJS) $(HEADERS) $(LIB_PATH) -o $(NAME)

clean		:
					$(RM) $(OBJS) $(OBJS_BONUS)

fclean		:		clean
					$(RM) $(NAME)

re			:		fclean all
