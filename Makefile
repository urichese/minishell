# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   Makefile                                           :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2021/11/15 21:27:29 by mlothair          #+#    #+#             #
#   Updated: 202#   Updated: 2021/12/22 17:55:30 by mlothair         ###   ########.fr       #                                                              #
# ************************************************************************** #

SRC = main.c command_funcs.c error.c ft_echo.c ft_exec_paths.c ft_exec.c minilibft/ft_atoi.c minilibft/ft_isalnum.c minilibft/ft_itoa.c minilibft/ft_strjoin.c minilibft/ft_strlen.c ft_parser.c ft_lexer.c minilibft/ft_isdigit.c ft_expander.c builtins.c minilibft/ft_strcmp.c ft_exit.c ft_cd.c minilibft/ft_memcpy.c minilibft/ft_strlcpy.c minilibft/ft_strchr.c ft_export.c env_utils.c minilibft/ft_substr.c ft_pwd.c minilibft/ft_strdup.c minilibft/ft_strrchr.c minilibft/ft_strncmp.c ft_unset.c minilibft/ft_split.c
OBJS = $(SRC:.c=.o)
DEPS = $(SRC:.c=.d)
CC = gcc

CFLAGS  = -MMD

TARGET = minishell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LIBFT) $(CFLAGS) $(OBJS) -lreadline -lncurses -o $@

.PHONY: clean fclean all re

-include $(DEPS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)

fclean: clean
	rm -f $(TARGET)

re: fclean all