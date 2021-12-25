/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 21:39:19 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/03 21:27:33 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <sys/fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>

typedef unsigned char	t_bool;

typedef enum e_char_types
{
	_ordinary = -1,
	_double_semicolon = -4,
	_double_greater = -3,
	_io = -2,
	_token = -1,
	_pipe = '|',
	_quote = '\'',
	_double_quote = '\"',
	_semicolon = ';',
	_space = ' ',
	_escape = '\\',
	_greater = '>',
	_lesser = '<',
	_tab = '\t',
}			t_char_types;

typedef struct s_token
{
	struct s_token	*next;
	struct s_token	*prev;
	t_char_types	type;
	char			*data;
}				t_token;

typedef enum e_redirect_type
{
	_red_input,
	_red_output,
	_red_d_output
}			t_redirect_type;

typedef struct s_redirect
{
	int					fd_io;
	int					fd_file;
	int					fd_backup;
	t_redirect_type		type;
	t_token				*filename;
	struct s_redirect	*next;
	struct s_redirect	*prev;
}				t_redirect;

typedef struct s_pre_command
{
	t_token					*args;
	t_redirect				*redirects;
	pid_t					pid;
	struct s_pre_command	*next;
}				t_pre_command;

typedef struct s_environments
{
	char					*name;
	char					*value;
	struct s_environments	*next;
	t_bool					is_environment;
}			t_environments;

typedef struct s_statuses
{
	t_environments	*env;
	int				last_status;
	char			*pwd;
	t_bool			is_exited;
}				t_statuses;

typedef enum e_pipe_statuses
{
	_empty,
	_rd_only,
	_wr_only,
	_rd_wr
}			t_pipe_statuses;

typedef enum e_state {
	in_dquoted,
	in_quoted,
	in_ordinarized,
}			t_state;

typedef struct s_tokeniser{
	size_t	str_i;
	size_t	tok_i;
	size_t	str_len;
	t_bool	esc_flag;
	t_bool	is_quoted;
	t_token	*token;
	t_token	*tokens_start;
	t_state	state;
}				t_tokeniser;

typedef enum e_node_type
{
	_node_type_cmd,
	_node_type_pipe,
	_node_type_semicol,
}			t_node_type;

typedef struct s_node
{
	t_node_type		type;
	t_pre_command	*command;
	struct s_node	*left;
	struct s_node	*right;
}				t_node;

t_environments	*create_new_env(char *env_str);
t_environments	*get_env(const char *name);
t_char_types	detect_type_of_token(char c);
void			add_env(t_environments **envs, t_environments *new_env);
void			free_names(t_node **node);
const char		*get_env_data(char *name);
int				ft_isalpha(int c);
int				print_envs(void);
t_bool			is_identifier(char *name);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*create_expanded_str(const char *str, t_state	state,
					t_bool is_env);
void			ft_safe_free_char(char **target);
int				ft_export(char **args);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
void			*ft_memcpy(void *dst, const void *src, size_t n);
char			*ft_strchr(const char *s, int c);
void			bind_pwd_value(void);
void			exec(char **args, pid_t pid);
char			*ft_strdup(const char *s1);
char			**ft_split(char const *s, char c);
char			*ft_strrchr(const char *s, int c);
void			smart_free(void **thing);
int				ft_pwd(void);
int				ft_unset(char **args);
t_bool			ft_parser(t_node **nodes, t_token **tokens);
void			ft_expander(t_token **tkns);
void			update_env_value(const char *env_name, const char *new_value,
					t_bool is_env_var, t_bool append_flag);
void			free_tokens(t_token **token_p);
char			*ft_substr(char const *s, unsigned int start, size_t len);
t_token			*ft_lexer(char *str, t_bool esc_flag);
void			set_signal_handler(void (*func)(int));
int				run_embedded(char **args);
int				ft_strcmp(char *s1, char *s2);
int				ft_echo(char **args);
int				ft_atoi(const char *str);
int				ft_exit(char **args);
int				ft_cd(char **args);
int				ft_isdigit(int c);
void			print_syntax_error(t_token *token);
t_token			*token_init(size_t len, t_token *prev);
t_bool			dup_redirects(t_pre_command *command, t_bool is_parent);
void			dup_pipe(t_pipe_statuses state, int old_pipe[], int new_pipe[]);
t_bool			setup_redirects(t_pre_command	*command);
int				ft_isalnum(int c);
size_t			ft_strlen(const char *s);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_itoa(int n);
t_bool			ft_atoi_with_limit(const char *str, int *return_value);
void			ft_exec(char *line);
void			err_ext(void);
int				ft_execvp(const char *file, char *const argv[]);

#endif
