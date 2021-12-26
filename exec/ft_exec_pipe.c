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

static void	new_pipe_states(t_pre_command	*command, t_pipe_statuses *state)
{
	if (*state == _empty)
		return ;
	if (command->next && command->next->next)
		*state = _rd_wr;
	else
		*state = _rd_only;
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

