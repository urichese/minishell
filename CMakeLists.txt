cmake_minimum_required(VERSION 3.20)
project(minishell C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-lreadline -lncurses")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(minishell main.c error.c minishell.h command_funcs.c minilibft/ft_itoa.c minilibft/ft_atoi.c ft_exec_paths.c error.c ft_echo.c ft_exec.c minilibft/ft_strlen.c minilibft/ft_isalnum.c ft_parser.c ft_lexer.c minilibft/ft_isdigit.c ft_expander.c builtins.c minilibft/ft_strcmp.c ft_exit.c ft_cd.c minilibft/ft_memcpy.c minilibft/ft_strlcpy.c minilibft/ft_strchr.c minilibft/ft_strjoin.c ft_export.c env_utils.c minilibft/ft_substr.c ft_pwd.c minilibft/ft_split.c minilibft/ft_strdup.c minilibft/ft_strrchr.c minilibft/ft_strncmp.c ft_unset.c)
