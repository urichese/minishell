/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 03:36:29 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	refresh_redir(t_cmd	*command)
{
	t_redirect	*redir;

	redir = command->redirects;
	while (redir && redir->next)
		redir = redir->next;
	while (redir)
	{
		if (redir->fd_file >= 0)
		{
			if (close(redir->fd_file) < 0)
				err_ext();
		}
		if (redir->fd_backup >= 0)
		{
			if (dup2(redir->fd_backup, redir->fd_io) < 0
				|| close(redir->fd_backup) < 0)
				err_ext();
		}
		redir = redir->prev;
	}
}

static t_bool	check_redirect(t_redirect *redir)
{
	if (redir->filename == 0 || redir->filename->next)
	{
		perror("redirect error");
		return (0);
	}
	redir->fd_file = open_file(redir);
	if (redir->fd_file < 0)
	{
		perror(redir->filename->data);
		return (0);
	}
	return (1);
}

t_bool	setup_redirects(t_cmd	*command)
{
	t_redirect	*redir;
	char		*org_filename;

	redir = command->redirects;
	while (redir)
	{
		org_filename = ft_strdup(redir->filename->data);
		if (org_filename == 0)
			err_ext();
		ft_expander(&redir->filename);
		if (check_redirect(redir) == 0)
		{
			free(org_filename);
			refresh_redir(command);
			return (0);
		}
		free(org_filename);
		redir = redir->next;
	}
	return (1);
}

void	dup_pipe(t_pipe_stats state, int old_pipe[], int new_pipe[])
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

t_bool	dup_redirects(t_cmd *command, t_bool is_parent)
{
	t_redirect	*redir;

	redir = command->redirects;
	while (redir)
	{
		if (is_parent)
		{
			redir->fd_backup = dup(redir->fd_io);
			if (redir->fd_backup < 0)
			{
				print_bad_fd_error(redir->fd_io);
				return (0);
			}
		}
		if (dup2(redir->fd_file, redir->fd_io) < 0)
		{
			print_bad_fd_error(redir->fd_io);
			return (0);
		}
		redir = redir->next;
	}
	return (1);
}
