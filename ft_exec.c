/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 02:48:11 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_signal_handler(void (*func)(int))
{
	if (signal(SIGINT, func) == SIG_ERR)
		err_ext();
	if (signal(SIGQUIT, func) == SIG_ERR)
		err_ext();
}

void	ft_printstr_nl(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

static void	run_from_node(t_node *nodes)
{
	extern t_statuses	g_states;
	t_pipe_stats		pipe_state;

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
	t_token				*tokens;
	t_token				*start_token;
	t_node				*nodes;
	extern t_statuses	g_states;

	nodes = 0;
	if (line == 0)
	{
		printf("\033[A");
		rl_replace_line("", 1);
		rl_redisplay();
		printf("\rminishell\u263A> exit\n");
		exit(g_states.last_status);
	}
	if (ft_strcmp(line, "") == 0)
		return ;
	tokens = ft_lexer(line, 0);
	start_token = tokens;
	if (!ft_parser(&nodes, &tokens))
		print_syntax_error(tokens);
	else
		nodes_recursive_run(nodes);
	free_tokens(&start_token);
	free_names(&nodes);
}
