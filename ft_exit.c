/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_digit_str(char *str)
{
	size_t	index;
	t_bool	has_digit;

	index = 0;
	has_digit = 0;
	while ((str[index]) == ' ')
		index++;
	if (str[index] == '+' || str[index] == '-')
		index++;
	while (str[index])
	{
		if (ft_isdigit(str[index]))
			has_digit = 1;
		else
			break ;
		index++;
	}
	while (str[index] == ' ' || str[index] == '\t')
		index++;
	if (str[index] == '\0' && has_digit == 1)
		return (1);
	else
		return (0);
}

static t_bool	has_error(char **args, int index)
{
	if (errno || is_digit_str(args[index]) == 0)
	{
		perror("exit : argument is invalid");
		exit(255);
	}
	if (args[index + 1])
	{
		perror("exit : too many arguments");
		return (1);
	}
	return (0);
}

int	ft_exit(char **args)
{
	extern t_statuses	g_states;
	int					i;
	int					status;

	i = 1;
	if (args[i] && ft_strcmp(args[i], "--") == 0)
		i++;
	if (args[i] == NULL)
		exit(g_states.last_status);
	errno = 0;
	status = ft_atoi(args[i]);
	if (has_error(args, i) == 0)
		exit(status);
	g_states.is_exited = 1;
	return (1);
}
