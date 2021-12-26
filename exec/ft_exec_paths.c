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

#include <alloca.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <paths.h>

void	for_scripts(const char *file, char *const argv[],
					int argc, char **new_argv)
{
	new_argv[0] = (char *) _PATH_BSHELL;
	new_argv[1] = (char *) file;
	while (argc > 1)
	{
		new_argv[argc] = argv[argc - 1];
		--argc;
	}
}

size_t	string_preparation(const char *file, char **path, char **name)
{
	size_t	pathlen;
	size_t	len;
	size_t	alloclen;

	alloclen = 0;
	*path = getenv ("PATH");
	if (*path == 0)
	{
		pathlen = confstr (_CS_PATH, (char *) 0, 0);
		alloclen = pathlen + 1;
	}
	else
		pathlen = ft_strlen (*path);
	len = ft_strlen (file) + 1;
	alloclen += pathlen + len + 1;
	*name = malloc (alloclen);
	if (*name == 0)
		return (0);
	if (*path == 0)
	{
		*path = *name + pathlen + len + 1;
		*path[0] = ':';
		(void) confstr (_CS_PATH, *path + 1, pathlen);
	}
	*name = (char *) ft_memcpy (*name + pathlen + 1, file, len);
	*--*name = '/';
	return (1);
}

void	do_not_search(const char *file, char *const argv[])
{
	char	**script_argv;
	void	*ptr;
	int		argc;
	size_t	len;

	ptr = 0;
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

int	path_search(char *path, char *name, char *const argv[])
{
	char	**script_argv;
	void	*script_argv_malloc;
	t_bool	got_eacces;
	char	*p;
	char	*startp;
	int		argc;

	p = path;
	script_argv = 0;
	script_argv_malloc = 0;
	got_eacces = 0;
	do
	{
	path = p;
	p = ft_strchr (path, ':');
	if (p == path)
		startp = name + 1;
	else
		startp = (char *) ft_memcpy (name - (p - path), path, p - path);
	execve (startp, argv, 0);
	if (errno == ENOEXEC)
	{
		if (script_argv == 0)
		{
			argc = 0;
			while (argv[argc++])
				;
			size_t arglen = (argc + 1) * sizeof (char *);
			script_argv_malloc = malloc (arglen);
			script_argv = script_argv_malloc;
			if (script_argv == 0)
			{
				got_eacces = 0;
				break;
			}
			for_scripts(startp, argv, argc, script_argv);
		}
		execve (script_argv[0], script_argv, 0);
	}
	if (errno == EACCES)
		got_eacces = 1;
	if (errno == ETIMEDOUT)
		break ;
	}
	while (*p++ != '\0');
	free (script_argv_malloc);
	return (got_eacces);
}

int	ft_execvp(const char *file, char *const argv[])
{
	char	*path;
	char	*name;

	if (*file == '\0')
	{
		errno = ENOENT;
		return (-1);
	}
	if (ft_strchr (file, '/') != 0)
		do_not_search(file, argv);
	else
	{
		if (!string_preparation(file, &path, &name))
		{
			smart_free((void **) &name);
			smart_free((void **) &path);
			return (-1);
		}
		if (path_search(path, name, argv))
			errno = EACCES;
		smart_free((void **) &name);
		smart_free((void **) &path);
	}
	return (-1);
}
