/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_conversations.c                            :+:      :+:    :+:   */
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

char	**convert_args(t_cmd *command)
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

t_bool	convert_tokens(t_cmd *command, char ***args)
{
	ft_expander(&command->args);
	*args = convert_args(command);
	if (*args[0] == 0)
		return (0);
	return (1);
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
