SRC = main.c command_funcs.c error.c ft_echo.c ft_exec_paths.c ft_exec.c minilibft/ft_atoi.c minilibft/ft_isalnum.c minilibft/ft_itoa.c minilibft/ft_strjoin.c minilibft/ft_strlen.c ft_parser.c ft_lexer.c minilibft/ft_isdigit.c ft_expander.c embedded.c minilibft/ft_strcmp.c ft_exit.c ft_cd.c minilibft/ft_memcpy.c minilibft/ft_strlcpy.c minilibft/ft_strchr.c ft_export.c env_utils.c minilibft/ft_substr.c ft_pwd.c minilibft/ft_strdup.c minilibft/ft_strrchr.c minilibft/ft_strncmp.c ft_unset.c minilibft/ft_split.c ft_env.c ft_parser_tokens.c ft_parser_redirects.c ft_parser_nodes.c ft_lexer_tokens.c ft_lexer_tokens_utils.c ft_lexer_states.c env_utils_sorting.c env_utils_val_ops.c ft_exec_conversations.c ft_exec_pipes_redir.c ft_exec_pipes_utils.c ft_exec_exec.c ft_exec_run_child.c ft_cd_change.c ft_cd_utils.c ft_cd_try_dirs.c ft_cd_colons.c ft_expander_token_work.c ft_expander_env_var.c ft_exec_paths_search.c main_inits.c
OBJS = $(SRC:.c=.o)
DEPS = $(SRC:.c=.d)
CC = gcc

CFLAGS = -MMD -Wall -Wextra -Werror

TARGET = minishell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -L/Users/mlothair/homebrew/Cellar/readline/8.1.1/lib/ -I/Users/mlothair/homebrew/Cellar/readline/8.1.1/include/ -lreadline $(OBJS) -o $@

.PHONY: clean fclean all re

-include $(DEPS)

.c.o:
	$(CC) $(CFLAGS) -I/Users/mlothair/homebrew/Cellar/readline/8.1.1/include/ -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)

fclean: clean
	rm -f $(TARGET)

re: fclean all
