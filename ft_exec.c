/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 18:34:19 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	calc_args(t_token *args)
{
	size_t	len;

	len = 0;
	while (args)
	{
		args = args->next;
		len++;
	}
	return (len);
}

static char	**convert_args(t_pre_command *command)
{
	char	**args;
	t_token	*now_token;
	size_t	len;
	size_t	i;

	len = calc_args(command->args);
	args = (char **)malloc(sizeof(char *) * (len + 1));
	if (!args)
		err_ext();
	now_token = command->args;
	i = 0;
	while (now_token)
	{
		args[i] = strdup(now_token->data);
		if (!args[i])
			err_ext();
		now_token = now_token->next;
		i++;
	}
	args[i] = 0;
	return (args);
}

t_bool	convert_tokens(t_pre_command *command, char ***args)
{
	ft_expander(&command->args);
	*args = convert_args(command);
	if (*args[0] == 0)
		return (0);
	return (1);
}

void	refresh_redir(t_pre_command	*command)
{
	t_redirect	*redir;

	redir = command->redirects;
	while (redir && redir->next)
		redir = redir->next;
	while (redir)
	{
		if (redir->fd_file >= 0)
		{
			if (close(redir->fd_file) < 0)
				err_ext();
		}
		if (redir->fd_backup >= 0)
		{
			if (dup2(redir->fd_backup, redir->fd_io) < 0
				|| close(redir->fd_backup) < 0)
				err_ext();
		}
		redir = redir->prev;
	}
}

static void	new_pipe_states(t_pre_command	*command, t_pipe_statuses *state)
{
	if (*state == _empty)
		return ;
	if (command->next && command->next->next)
		*state = _rd_wr;
	else
		*state = _rd_only;
}

void	free_arr(char ***arr_ptr)
{
	int	i;

	i = 0;
	if (!*arr_ptr)
		return ;
	while ((*arr_ptr)[i])
	{
		free((*arr_ptr)[i]);
		(*arr_ptr)[i] = 0;
		i++;
	}
	free(*arr_ptr);
	*arr_ptr = 0;
}

static t_bool	can_write(t_pipe_statuses state)
{
	if (state == _wr_only || state == _rd_wr)
		return (1);
	return (0);
}

void	create_pipe(t_pipe_statuses state, int new_pipe[])
{
	if (can_write(state) == 1)
	{
		if (pipe(new_pipe) < 0)
			err_ext();
	}
}

void	set_signal_handler(void (*func)(int))
{
	if (signal(SIGINT, func) == SIG_ERR)
		err_ext();
	if (signal(SIGQUIT, func) == SIG_ERR)
		err_ext();
}

static t_bool	can_read(t_pipe_statuses state)
{
	if (state == _rd_only || state == _rd_wr)
		return (1);
	return (0);
}

void	cleanup_pipe(t_pipe_statuses state, int old_pipe[], int new_pipe[])
{
	if (can_read(state) == 1)
	{
		if (close(old_pipe[0]) < 0 || close(old_pipe[1]) < 0)
			err_ext();
	}
	if (can_write(state) == 1)
	{
		old_pipe[0] = new_pipe[0];
		old_pipe[1] = new_pipe[1];
	}
}

static int	open_file(t_redirect *redir)
{
	char	*filename;

	filename = redir->filename->data;
	if (redir->type == _red_input)
		return (open(filename, O_RDONLY));
	if (redir->type == _red_output)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

static t_bool	check_redirect(t_redirect *redir, char *org_filename)
{
	if (redir->filename == NULL || redir->filename->next)
	{
		perror("redirect error");
		return (0);
	}
	redir->fd_file = open_file(redir);
	if (redir->fd_file < 0)
	{
		perror(strerror(errno));
		return (0);
	}
	return (1);
}

t_bool	setup_redirects(t_pre_command	*command)
{
	t_redirect	*redir;
	char		*org_filename;

	redir = command->redirects;
	while (redir)
	{
		org_filename = strdup(redir->filename->data);
		if (org_filename == NULL)
			err_ext();
		ft_expander(&redir->filename);
		if (check_redirect(redir, org_filename) == 0)
		{
			free(org_filename);
			refresh_redir(command);
			return (0);
		}
		free(org_filename);
		redir = redir->next;
	}
	return (1);
}

void	dup_pipe(t_pipe_statuses state, int old_pipe[], int new_pipe[])
{
	if (can_read(state) == 1)
	{
		if (close(old_pipe[1]) < 0
			|| dup2(old_pipe[0], 0) < 0
			|| close(old_pipe[0]) < 0)
			err_ext();
	}
	if (can_write(state) == 1)
	{
		if (close(new_pipe[0]) < 0
			|| dup2(new_pipe[1], 1) < 0
			|| close(new_pipe[1]) < 0)
			err_ext();
	}
}

void	print_bad_fd_error(int fd)
{
	char	*fd_str;

	if (fd < 0)
	{
		perror(strerror(errno));
		perror("file descriptor out of range");
	}
	else
	{
		fd_str = ft_itoa(fd);
		if (!fd_str)
			err_ext();
		perror(strerror(errno));
		perror(fd_str);
		free(fd_str);
	}
}

t_bool	dup_redirects(t_pre_command *command, t_bool is_parent)
{
	t_redirect	*redir;

	redir = command->redirects;
	while (redir)
	{
		if (is_parent)
		{
			redir->fd_backup = dup(redir->fd_io);
			if (redir->fd_backup < 0)
			{
				print_bad_fd_error(redir->fd_io);
				return (0);
			}
		}
		if (dup2(redir->fd_file, redir->fd_io) < 0)
		{
			print_bad_fd_error(redir->fd_io);
			return (0);
		}
		redir = redir->next;
	}
	return (1);
}

int	is_builtin(char **args)
{
	if (args[0] == 0)
		return (0);
	if (!strcmp(args[0], "echo")
		|| !strcmp(args[0], "cd")
		|| !strcmp(args[0], "pwd")
		|| !strcmp(args[0], "export")
		|| !strcmp(args[0], "unset")
		|| !strcmp(args[0], "env")
		|| !strcmp(args[0], "exit"))
		return (1);
	return (0);
}

static void	exec_command_child(
		t_pre_command *command, char **args,
		t_pipe_statuses state, int old_pipe[])
{
	pid_t	pid;
	int		new_pipe[2];

	create_pipe(state, new_pipe);
	pid = fork();
	if ((pid) < 0)
		err_ext();
	if (pid == 0)
	{
		set_signal_handler(SIG_DFL);
		if (setup_redirects(command) == 0)
			exit(1);
		if (args[0] == 0)
			exit(0);
		dup_pipe(state, old_pipe, new_pipe);
		if (dup_redirects(command, 0) == 0)
			exit(1);
		if (is_builtin(args))
			exit(run_embedded(args));
		else
			exec(args, pid);
	}
	set_signal_handler(SIG_IGN);
	cleanup_pipe(state, old_pipe, new_pipe);
	command->pid = pid;
}

static int	exec_builtin_parent(t_pre_command *command, char **args)
{
	if (setup_redirects(command) == 0)
		return (1);
	if (dup_redirects(command, 1) == 0)
		return (1);
	return (run_embedded(args));
}

int	exec_command(t_pre_command *command, t_pipe_statuses *state, int old_pipe[])
{
	char	**args;
	int		status;

	status = 0;
	convert_tokens(command, &args);
	if (*state == _empty && is_builtin(args))
		status = exec_builtin_parent(command, args);
	else
		exec_command_child(command, args, *state, old_pipe);
	refresh_redir(command);
	new_pipe_states(command, state);
	free_arr(&args);
	return (status);
}

void	wait_commands(t_pre_command *command)
{
	int			status;
	t_bool		has_child;
	t_bool		catch_sigint;

	has_child = 0;
	catch_sigint = 0;
	while (command)
	{
		if (command->pid != -1)
		{
			if (waitpid(command->pid, &status, 0) < 0)
				err_ext(NULL);
			if (WIFSIGNALED(status) && WTERMSIG(status) == 2)
				catch_sigint = 1;
			has_child = 1;
		}
		command = command->next;
	}
	if (has_child == 0)
		return ;
	if (catch_sigint)
		printf("\n");
}

static void	exec_pipeline(t_node *nodes)
{
	extern t_statuses	g_states;
	t_pre_command		*command;
	int					pipe[2];
	t_pipe_statuses		pipe_state;

	pipe_state = _wr_only;
	while (nodes->type == _node_type_pipe)
		nodes = nodes->left;
	command = nodes->command;
	while (command)
	{
		g_states.last_status = exec_command(command, &pipe_state, pipe);
		command = command->next;
	}
	wait_commands(nodes->command);
}

static void	run_from_node(t_node *nodes)
{
	extern t_statuses	g_states;
	t_pipe_statuses		pipe_state;

	pipe_state = _empty;
	if (!nodes)
		return ;
	if (nodes->type == _node_type_pipe)
		exec_pipeline(nodes);
	else
	{
		g_states.last_status = exec_command(nodes->command, &pipe_state, 0);
		wait_commands(nodes->command);
	}
}

void	nodes_recursive_run(t_node *nodes)
{
	extern t_statuses	g_states;

	if (!nodes || g_states.is_exited == 1)
		return ;
	if (nodes->type == _node_type_semicol)
	{
		nodes_recursive_run(nodes->left);
		nodes_recursive_run(nodes->right);
	}
	else
		run_from_node(nodes);
}

void	ft_exec(char *line)
{
	t_token	*tokens;
	t_token	*start_token;
	t_node	*nodes;

	tokens = ft_lexer(line, 0);
	start_token = tokens;
	if (!ft_parser(&nodes, &tokens))
		print_syntax_error(tokens);
	else
		nodes_recursive_run(nodes);
	free_tokens(&start_token);
	free_names(&nodes);
}
