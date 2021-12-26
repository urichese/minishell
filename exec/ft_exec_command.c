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



