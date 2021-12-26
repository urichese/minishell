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

static int	open_file(t_redirect *redir)
{
	char	*filename;

	filename = redir->filename->data;
	if (redir->type == _red_input)
		return (open(filename, O_RDONLY));
	if (redir->type == _red_output)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

static t_bool	check_redirect(t_redirect *redir, char *org_filename)
{
	if (redir->filename == NULL || redir->filename->next)
	{
		perror("redirect error");
		return (0);
	}
	redir->fd_file = open_file(redir);
	if (redir->fd_file < 0)
	{
		perror(strerror(errno));
		return (0);
	}
	return (1);
}

t_bool	setup_redirects(t_pre_command	*command)
{
	t_redirect	*redir;
	char		*org_filename;

	redir = command->redirects;
	while (redir)
	{
		org_filename = strdup(redir->filename->data);
		if (org_filename == NULL)
			err_ext();
		ft_expander(&redir->filename);
		if (check_redirect(redir, org_filename) == 0)
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

void	refresh_redir(t_pre_command	*command)
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

t_bool	dup_redirects(t_pre_command *command, t_bool is_parent)
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
