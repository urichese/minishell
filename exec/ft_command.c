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



