/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 03:37:22 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_pipe_states(t_cmd	*command, t_pipe_stats *state)
{
	if (*state == _empty)
		return ;
	if (command->next && command->next->next)
		*state = _rd_wr;
	else
		*state = _rd_only;
}

t_bool	can_write(t_pipe_stats state)
{
	if (state == _wr_only || state == _rd_wr)
		return (1);
	return (0);
}

void	create_pipe(t_pipe_stats state, int new_pipe[])
{
	if (can_write(state) == 1)
	{
		if (pipe(new_pipe) < 0)
			err_ext();
	}
}

t_bool	can_read(t_pipe_stats state)
{
	if (state == _rd_only || state == _rd_wr)
		return (1);
	return (0);
}

void	cleanup_pipe(t_pipe_stats state, int old_p[], int new_p[])
{
	if (can_read(state) == 1)
	{
		if (close(old_p[0]) < 0 || close(old_p[1]) < 0)
			err_ext();
	}
	if (can_write(state) == 1)
	{
		old_p[0] = new_p[0];
		old_p[1] = new_p[1];
	}
}
