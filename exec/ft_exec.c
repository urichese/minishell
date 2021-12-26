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

void	set_signal_handler(void (*func)(int))
{
	if (signal(SIGINT, func) == SIG_ERR)
		err_ext();
	if (signal(SIGQUIT, func) == SIG_ERR)
		err_ext();
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
