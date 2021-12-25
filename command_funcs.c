/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:12:06 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/28 20:10:11 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(char **args, pid_t pid)
{
	int					status;
	extern t_statuses	g_states;
	int					signal;

	if (pid == 0)
	{
		ft_execvp(args[0], args);
		perror(args[0]);
		exit(255);
	}
	else if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (waitpid(pid, &status, 0) < 0)
	{
		perror("wait");
		exit(254);
	}
	if (WIFEXITED(status))
		g_states.last_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		g_states.last_status = signal + 128;
		printf("Process %d killed: signal %d", pid, WTERMSIG(status));
		if (WCOREDUMP(status))
			printf(" - core dumped\n");
		else
			printf("\n");
	}
}
