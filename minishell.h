/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 21:39:19 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 04:23:55 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <sys/fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>

typedef unsigned char	t_bool;

typedef enum e_char_types
{
	_ordinary = -1,
	_double_semicolon = -4,
	_double_greater = -3,
	_double_lesser = -5,
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
	_red_d_input,
	_red_output,
	_red_d_output
}						t_redirect_type;

typedef struct s_redirect
{
	int					fd_io;
	int					fd_file;
	int					fd_backup;
	t_redirect_type		type;
	t_token				*filename;
	struct s_redirect	*next;
	struct s_redirect	*prev;
}						t_redirect;

typedef struct s_pre_command
{
	t_token					*args;
	t_redirect				*redirects;
	pid_t					pid;
	struct s_pre_command	*next;
}						t_cmd;

typedef struct s_environments
{
	char					*name;
	char					*value;
	struct s_environments	*next;
	t_bool					is_environment;
}				t_environments;

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
}			t_pipe_stats;

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
}				t_node_type;

typedef struct s_node
{
	t_node_type		type;
	t_cmd			*command;
	struct s_node	*left;
	struct s_node	*right;
}				t_node;

typedef struct s_expander
{
	size_t			str_i;
	t_state			state;
	t_char_types	type;
	char			*str;
}				t_expander;

void			detect_char_types(t_char_types	*type, \
				t_tokeniser *toker, char *str);
void			do_not_search(const char *file, char *const argv[]);
char			**generate_environ(t_environments *envs);
int				path_search(char *path, char *name, char *const argv[]);
void			ft_heredoc(char *stop_word, int fd);
void			ft_putstr_fd(char *s, int fd);
void			expander_init(t_expander *exper, char *input);
t_state			judge_token_state(t_state	state, t_char_types type);
void			token_join(t_token *prev_token, t_token *next_token);
t_token			*find_last_token(t_token *tkns);
char			*expand_env_var(char *input);
char			*dup_env_value(char *name);
size_t			calc_escaped_value_len(const char *str, const char *esc);
void			copy_escaped_value(const char *src, const char *esc, \
				char *dest);
void			fill_environ_i(char **env_i, t_environments *envs);
void			general_state(t_tokeniser *toker, t_char_types	type, \
				char *str);
void			d_quote_state(t_tokeniser *toker, t_char_types type, char *str);
void			quote_state(t_tokeniser *toker, t_char_types type, char *str);
void			ft_sorting(t_environments **lst);
void			free_arr(char ***arr_ptr);
t_bool			convert_tokens(t_cmd *command, char ***args);
void			wait_commands(t_cmd *command);
void			exec_pipeline(t_node *nodes);
void			ft_printstr_nl(char *s, int fd);
int				exec_command(t_cmd *cmd, t_pipe_stats *state, int old_pipe[]);
void			create_pipe(t_pipe_stats state, int new_pipe[]);
void			cleanup_pipe(t_pipe_stats state, int old_p[], int new_p[]);
void			new_pipe_states(t_cmd	*command, t_pipe_stats *state);
int				open_file(t_redirect *redir);
void			print_bad_fd_error(int fd);
t_bool			can_write(t_pipe_stats state);
t_bool			can_read(t_pipe_stats state);
int				is_embedded(char **args);
void			exec_command_child(t_cmd *cmd, char **args, \
				t_pipe_stats state, int old_pipe[]);
int				exec_builtin_parent(t_cmd *command, char **args);
void			refresh_redir(t_cmd	*command);
char			*join_path(const char *prev, const char *next);
int				chng_dir_process(char *cd_path, const char *arg, \
				t_bool is_orig_path);
void			add_slash_path_front(char *path, char **res);
t_bool			cpy_canonical_path(char **split, char **res);
char			*path_canonicalisation(char *path);
t_bool			needs_env_path_search(char **args, const char *dest);
t_bool			try_change_dir(const char *destination);
void			ft_safe_free_split(char ***target);
t_bool			try_env_path(const char *dest);
const char		*set_cd_destination(char **args);
char			**get_colon_units(const char *str, const char *default_value);
char			*set_cd_path(const char *arg, t_bool *is_canon_path);
t_token			*token_init(size_t len, t_token *prev);
t_bool			is_io_number_token(t_tokeniser *toker, t_char_types type);
void			close_token_list(t_tokeniser *toker);
void			del_token(t_token **token_p);
void			tokeniser_add_new_token(t_tokeniser *toker);
void			tokeniser_init(t_tokeniser *toker, char *str, t_bool esc_flag);
t_node			*create_command_node(t_cmd **cmd_for_node);
t_node			*add_parent_node(t_node_type type, t_node *left, t_node *right);
t_bool			parse_command(t_cmd	**cmd, t_node **node, t_token **tokens);
t_bool			parse_io_redirect(t_token **tkns, t_node *command_node);
t_bool			is_redirect_token(t_token *tkn);
int				ft_env(void);
t_bool			set_redirect_type(t_token *tkn, t_redirect *redirect);
void			add_redirect(t_redirect **redir_lst, t_redirect *new_elem);
t_redirect		*create_redirect(void);
void			del_redirect_list(t_redirect **redirect_p);
void			set_command_args(t_cmd *command, t_token **tokens);
t_bool			has_token_type(t_token **tok, t_char_types tok_type);
void			free_names(t_node **node);
void			add_copied_token(t_token **list, t_token *original_token);
void			fill_envs(void);
t_bool			is_same_dir(const char *dir_1, const char *dir_2);
t_bool			is_directory(const char *path);
t_environments	*create_environment(char *env_str);
t_environments	*get_env(const char *name);
void			add_env(t_environments **envs, t_environments *new_env);
const char		*get_env_data(char *name);
int				ft_isalpha(int c);
int				ft_isspace(int c);
void			initialize_old_pwd(void);
int				ft_atoi_over(const char *str);
void			calc_shlvl(char **shlvl);
int				print_envs(void);
t_bool			is_identifier(char *name);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*create_expand_str(const char *str, t_state	state, \
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
void			update_env_value(const char *env_name, const char *new_value, \
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
t_char_types	detect_type_of_token(char c);
void			print_syntax_error(t_token *token);
t_bool			dup_redirects(t_cmd *command, t_bool is_parent);
void			dup_pipe(t_pipe_stats state, int old_pipe[], int new_pipe[]);
t_bool			setup_redirects(t_cmd	*command);
int				ft_isalnum(int c);
size_t			ft_strlen(const char *s);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_itoa(int n);
t_bool			ft_atoi_by_ref(const char *str, int *return_value);
void			ft_exec(char *line);
void			err_ext(void);
int				ft_execvp(const char *file, char *const argv[]);

#endif
