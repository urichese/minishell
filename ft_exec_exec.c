/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 03:37:45 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_embedded(char **args)
{
	if (args[0] == 0)
		return (0);
	if (!ft_strcmp(args[0], "echo")
		|| !ft_strcmp(args[0], "cd")
		|| !ft_strcmp(args[0], "pwd")
		|| !ft_strcmp(args[0], "export")
		|| !ft_strcmp(args[0], "unset")
		|| !ft_strcmp(args[0], "env")
		|| !ft_strcmp(args[0], "exit"))
		return (1);
	return (0);
}

int	exec_command(t_cmd *cmd, t_pipe_stats *state, int old_pipe[])
{
	char	**args;
	int		stat;

	stat = 0;
	convert_tokens(cmd, &args);
	if (*state == _empty && is_embedded(args))
		stat = exec_builtin_parent(cmd, args);
	else
		exec_command_child(cmd, args, *state, old_pipe);
	refresh_redir(cmd);
	new_pipe_states(cmd, state);
	free_arr(&args);
	return (stat);
}

void	refresh_status(int status, t_bool sigint, char *cmd_name)
{
	extern t_statuses	g_states;
	int					signal;

	if (WIFEXITED(status))
		g_states.last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		if (signal == 3)
			ft_printstr_nl("Quit: 3", 2);
		else if (signal == 11)
		{
			write(2, "minishell: ", 11);
			if (cmd_name)
			{
				write(2, cmd_name, ft_strlen(cmd_name));
				write(2, ": ", 2);
			}
			ft_printstr_nl("command not found", 2);
		}
		g_states.last_status = signal + 128;
	}
	if (sigint)
		ft_printstr_nl("", 2);
}

void	wait_commands(t_cmd *command)
{
	int		status;
	t_bool	has_child;
	t_bool	sigint;
	char	*name;

	has_child = 0;
	sigint = 0;
	while (command)
	{
		if (command->pid != -1)
		{
			if (waitpid(command->pid, &status, 0) < 0)
				err_ext();
			if (WIFSIGNALED(status) && WTERMSIG(status) == 2)
				sigint = 1;
			has_child = 1;
			name = 0;
			if (command->args && command->args->data)
				name = command->args->data;
		}
		command = command->next;
	}
	if (has_child == 0)
		return ;
	refresh_status(status, sigint, name);
}

void	exec_pipeline(t_node *nodes)
{
	extern t_statuses	g_states;
	t_cmd				*command;
	int					pipe[2];
	t_pipe_stats		pipe_state;

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
