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
	int	status;

	if (pid == 0)
	{
		if (ft_execvp(args[0], args) < 0)
		{
			if (errno == ENOENT)
				status = 127;
			else
				status = 126;
			if (errno == ENOEXEC)
				exit(0);
			perror(args[0]);
			exit(status);
		}
		perror(args[0]);
		exit(255);
	}
}
