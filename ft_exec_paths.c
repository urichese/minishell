/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_paths.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 18:04:20 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 03:26:17 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	can_generate_environ(t_environments *env)
{
	if (env->value == 0)
		return (0);
	if (env->is_environment == 0)
		return (0);
	return (1);
}

size_t	get_environ_size(t_environments *envs)
{
	size_t	size;

	size = 0;
	while (envs)
	{
		if (can_generate_environ(envs))
			size++;
		envs = envs->next;
	}
	return (size);
}

char	**generate_environ(t_environments *envs)
{
	char	**environ;
	size_t	i;
	size_t	size;

	size = get_environ_size(envs);
	environ = malloc(sizeof(char *) * (size + 1));
	if (!environ)
		err_ext();
	i = 0;
	while (i < size)
	{
		if (can_generate_environ(envs))
		{
			fill_environ_i(&environ[i], envs);
			i++;
		}
		envs = envs->next;
	}
	environ[i] = 0;
	return (environ);
}

size_t	string_preparation(const char *file, char **path, char **name)
{
	size_t	path_len;
	size_t	len;
	size_t	malloc_len;

	malloc_len = 0;
	*path = (char *) get_env_data("PATH");
	if (*path == 0 || ft_strcmp(*path, "") == 0)
		return (0);
	else
		path_len = ft_strlen (*path);
	len = ft_strlen (file) + 1;
	malloc_len += path_len + len + 1;
	*name = malloc (malloc_len);
	if (*name == 0)
		return (2);
	*name = (char *) ft_memcpy (*name + path_len + 1, file, len);
	*--*name = '/';
	return (1);
}

int	ft_execvp(const char *file, char *const argv[])
{
	char	*path;
	char	*name;
	size_t	returned_val;

	if (*file == '\0')
	{
		errno = ENOENT;
		return (-1);
	}
	if (ft_strchr (file, '/') != 0)
		do_not_search(file, argv);
	else
	{
		returned_val = string_preparation(file, &path, &name);
		if (returned_val == 2)
			smart_free((void **) &path);
		if (returned_val != 1)
			return (-1);
		if (path_search(path, name, argv))
			errno = EACCES;
		smart_free((void **) &name);
		smart_free((void **) &path);
	}
	return (-1);
}
