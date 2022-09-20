/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:34:33 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcpy_forward(char *dest, char *src)
{
	size_t	index;

	index = 0;
	while (src[index])
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';
	return (&(dest[index]));
}

char	*cpy_path_elem(char *path_p, char *elem, char *start)
{
	if (ft_strcmp(elem, "..") == 0)
	{
		path_p = ft_strrchr(start, '/');
		if (!path_p)
			path_p = start;
		*path_p = '\0';
	}
	else if (ft_strcmp(elem, ".") != 0)
	{
		path_p = ft_strcpy_forward(path_p, "/");
		path_p = ft_strcpy_forward(path_p, elem);
	}
	return (path_p);
}

t_bool	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (0);
	if (S_ISDIR(path_stat.st_mode))
		return (1);
	return (0);
}

t_bool	cpy_canonical_path(char **split, char **res)
{
	char	*start;
	char	*path_p;
	size_t	index;

	start = *res;
	*start = '\0';
	index = 0;
	path_p = start;
	while (split[index])
	{
		path_p = cpy_path_elem(path_p, split[index], start);
		if (path_p == start)
			path_p = ft_strcpy_forward(path_p, "/");
		if (!is_directory(start))
			return (0);
		index++;
	}
	if (path_p == start)
		path_p = ft_strcpy_forward(path_p, "/");
	return (1);
}

void	add_slash_path_front(char *path, char **res)
{
	char	*added_res;

	if (!path || !res || !*res)
		return ;
	if (ft_strncmp(path, "//", 2) == 0 && path[2] != '/'
		&& ft_strncmp(*res, "//", 2) != 0)
	{
		added_res = ft_strjoin("/", *res);
		if (!added_res)
			err_ext();
		ft_safe_free_char(res);
		*res = added_res;
	}
}
