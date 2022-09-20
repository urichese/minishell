/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_paths.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 18:04:20 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 18:35:24 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	for_scripts(const char *file, char *const argv[],
					int argc, char **new_argv)
{
	new_argv[0] = "/bin/sh";
	new_argv[1] = (char *) file;
	while (argc > 1)
	{
		new_argv[argc] = argv[argc - 1];
		--argc;
	}
}

void	do_not_search(const char *file, char *const argv[])
{
	char	**script_argv;
	void	*ptr;
	int		argc;
	size_t	len;

	argc = 0;
	execve (file, argv, 0);
	if (errno == ENOEXEC)
	{
		while (argv[argc++])
			;
		len = (argc + 1) * sizeof (char *);
		ptr = malloc (len);
		script_argv = ptr;
		if (script_argv != 0)
		{
			for_scripts(file, argv, argc, script_argv);
			execve (script_argv[0], script_argv, 0);
			free (ptr);
		}
	}
}

t_bool	try_scripts(char *const argv[], char *startp, t_bool *got_eacces)
{
	size_t				arglen;
	void				*script_argv_malloc;
	static char			**script_argv;
	int					argc;
	extern t_statuses	g_states;

	if (script_argv == 0)
	{
		argc = 0;
		while (argv[argc++])
			;
		arglen = (argc + 1) * sizeof (char *);
		script_argv_malloc = malloc (arglen);
		script_argv = script_argv_malloc;
		if (script_argv == 0)
		{
			*got_eacces = 0;
			smart_free(&script_argv_malloc);
			return (0);
		}
		for_scripts(startp, argv, argc, script_argv);
	}
	execve (script_argv[0], script_argv, generate_environ(g_states.env));
	smart_free (&script_argv_malloc);
	return (1);
}

int	path_search(char *path, char *name, char *const argv[])
{
	t_bool				got_eacces;
	char				*p;
	char				*startp;
	extern t_statuses	g_states;

	p = path;
	got_eacces = 0;
	while (*p != '\0')
	{
		path = p;
		p = ft_strchr (path, ':');
		if (p == path)
			startp = name + 1;
		else
			startp = (char *) ft_memcpy (name - (p - path), path, p - path);
		execve (startp, argv, generate_environ(g_states.env));
		if (errno == ENOEXEC && !try_scripts(argv, startp, &got_eacces))
			break ;
		if (errno == EACCES)
			got_eacces = 1;
		if (errno == ETIMEDOUT)
			break ;
		p++;
	}
	return (got_eacces);
}
