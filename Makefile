# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cleblais <cleblais@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 08:01:18 by cleblais          #+#    #+#              #
#    Updated: 2023/03/31 14:12:29 by cleblais         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=		minishell

SRCS		=	main.c \
				errors/errors.c \
				free/free.c \
				init/init.c \
				lexer/env.c \
				lexer/lexer.c \
				lexer/t_lexer_utils.c \
				lexer/token.c \
				lexer/token1.c \
				lexer/token2.c \
				libft/ft_split.c \
				libft/ft_strdup.c \
				libft/ms_strjoin.c \
				libft/ft_strlcat.c \
				libft/ft_strlcpy.c \
				libft/ft_strlen.c \
				libft/ft_strtrim.c \
				libft/ft_strzero.c \
				libft/ft_substr.c \
				parser/parser.c \
				parser/quotes.c \
				utilities/stack_function.c \
				utilities/utilities.c \
				utilities/utilities_second.c \
				utilities/lst_utils.c \
				builtin/built_env.c
				

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
					$(RM) $(OBJS)

fclean		:		clean
					$(RM) $(NAME)

re			:		fclean all