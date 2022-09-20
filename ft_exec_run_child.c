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

int	open_file(t_redirect *redir)
{
	char	*filename;
	int		ret;

	filename = redir->filename->data;
	if (redir->type == _red_input)
		return (open(filename, O_RDONLY));
	if (redir->type == _red_output)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redir->type == _red_d_output)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (redir->type == _red_d_input)
	{
		ret = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (ret < 0)
			return (ret);
		ft_heredoc(filename, ret);
		close(ret);
		ret = open(".heredoc", O_RDONLY);
		unlink(".heredoc");
		return (ret);
	}
	return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

void	print_bad_fd_error(int fd)
{
	char	*fd_str;

	if (fd < 0)
		perror("file descriptor out of range");
	else
	{
		fd_str = ft_itoa(fd);
		if (!fd_str)
			err_ext();
		perror(fd_str);
		free(fd_str);
	}
}

void	ft_heredoc(char *stop_word, int fd)
{
	char	*line;

	line = readline("> ");
	while (line)
	{
		if (ft_strcmp(stop_word, line) == 0)
			break ;
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		line = readline("> ");
	}
	smart_free((void **) &line);
}

void	exec_command_child(
		t_cmd *cmd, char **args,
		t_pipe_stats state, int old_pipe[])
{
	pid_t	pid;
	int		new_pipe[2];

	create_pipe(state, new_pipe);
	pid = fork();
	if (pid < 0)
		err_ext();
	if (pid == 0)
	{
		set_signal_handler(SIG_DFL);
		if (setup_redirects(cmd) == 0)
			exit(1);
		if (args[0] == 0)
			exit(0);
		dup_pipe(state, old_pipe, new_pipe);
		if (dup_redirects(cmd, 0) == 0)
			exit(1);
		if (is_embedded(args))
			exit(run_embedded(args));
		else
			exec(args, pid);
	}
	set_signal_handler(SIG_IGN);
	cleanup_pipe(state, old_pipe, new_pipe);
	cmd->pid = pid;
}

int	exec_builtin_parent(t_cmd *command, char **args)
{
	if (setup_redirects(command) == 0)
		return (1);
	if (dup_redirects(command, 1) == 0)
		return (1);
	return (run_embedded(args));
}
