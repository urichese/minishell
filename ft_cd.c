/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:34:33 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cwd_path(char *caller)
{
	char *cwd;

	cwd = getcwd(0, 0);
	if (!cwd)
		printf("%s: error retrieving current directory: getcwd: cannot access parent directories: %s\n", caller, strerror(errno));
	return (cwd);
}

char	*join_path(const char *prev, const char *next)
{
	char	*tmp;
	char	*res;
	size_t	index;

	if (!prev || !next)
		return (NULL);
	tmp = NULL;
	res = NULL;
	tmp = ft_strjoin(prev, "/");
	if (!tmp)
		err_ext();
	index = ft_strlen(tmp);
	if (index >= 2 && tmp[index - 2] == '/')
		tmp[index - 1] = '\0';
	res = ft_strjoin(tmp, next);
	if (!res)
		err_ext();
	free(tmp);
	return (res);
}

char	*get_new_pwd(char *path, t_bool is_canon_path, t_bool is_abs_path)
{
	char	*new_pwd;

	new_pwd = NULL;
	if (is_abs_path)
	{
		if (is_canon_path == 0)
			new_pwd = get_cwd_path("cd");
		if (is_canon_path || new_pwd == NULL)
		{
			new_pwd = ft_strdup(path);
			if (!new_pwd)
				err_ext();
		}
	}
	else
	{
		new_pwd = get_cwd_path("cd");
		if (!new_pwd)
		{
			new_pwd = ft_strdup(path);
			if (!new_pwd)
				err_ext();
		}
	}
	return (new_pwd);
}
int	change_dir_process(char *cd_path, const char *arg, t_bool is_canon_path)
{
	int					res;
	int					err;
	extern t_statuses	g_states;

	res = chdir(cd_path);
	if (res == 0)
	{
		ft_safe_free_char(&g_states.pwd);
		g_states.pwd = get_new_pwd(cd_path, is_canon_path, 1);
		return (res);
	}
	err = errno;
	res = chdir(arg);
	if (res == 0)
	{
		ft_safe_free_char(&g_states.pwd);
		g_states.pwd = get_new_pwd(cd_path, is_canon_path, 0);
		return (res);
	}
	errno = err;
	res = EXIT_FAILURE;
	return (res);
}

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

void	ft_safe_free_split(char ***target)
{
	size_t index;

	index = 0;
	if (!*target)
		return ;
	while ((*target)[index])
	{
		free((*target)[index]);
		(*target)[index] = NULL;
		index++;
	}
	free(*target);
	*target = NULL;
}

char	*path_canonicalisation(char *path)
{
	char			**split;
	char			*res;

	if (!path)
		return (0);
	split = ft_split(path, '/');
	res = malloc(sizeof(char *) * (ft_strlen(path) + 1));
	if (!split || !res)
		err_ext();
	if (!(cpy_canonical_path(split, &res)))
		ft_safe_free_char(&res);
	ft_safe_free_split(&split);
	add_slash_path_front(path, &res);
	return (res);
}

char	*set_cd_path(const char *arg, t_bool *is_canon_path)
{
	char				*canon_path;
	char				*physical_path;
	extern t_statuses	g_states;

	if (*arg == '/')
		physical_path = ft_strdup(arg);
	else
		physical_path = join_path(g_states.pwd, arg);
	if (!physical_path)
		err_ext();
	canon_path = path_canonicalisation(physical_path);
	if (canon_path)
	{
		ft_safe_free_char(&physical_path);
		*is_canon_path = 1;
		return (canon_path);
	}
	else
	{
		ft_safe_free_char(&canon_path);
		*is_canon_path = 0;
		return (physical_path);
	}
}

const char	*set_cd_destination(char **args)
{
	t_environments	*home_env;
	size_t	index;

	index = 0;
	while (args[index])
		index++;
	if (index == 1)
	{
		home_env = get_env("HOME");
		if (!home_env)
		{
			perror("minishell: cd: HOME is not set");
			return (NULL);
		}
		if (!home_env->value)
			return ("");
		return (home_env->value);
	}
	return (args[1]);
}

t_bool	needs_env_path_search(char **args, const char *dest)
{
	if (args[1] == NULL || args[1][0] == '/')
		return (0);
	if (ft_strcmp((char *)dest, ".") == 0
		|| ft_strcmp((char *)dest, "..") == 0
		|| ft_strncmp((char *)dest, "./", 2) == 0
		|| ft_strncmp((char *)dest, "../", 3) == 0)
		return (0);
	return (1);
}

char	**allocate_colon_unit_parent(const char *str)
{
	size_t	index;
	size_t	colon_count;
	char	**res;

	index = 0;
	colon_count = 0;
	while (str[index])
	{
		if (str[index] == ':')
			colon_count++;
		index++;
	}
	res = malloc(sizeof(char *) * (colon_count + 2));
	if (!res)
		err_ext();
	res[colon_count + 1] = 0;
	return (res);
}

char	*strdup_colon_unit(char *unit, const char *default_value)
{
	char	*res;

	if (!unit)
		return (0);
	if (ft_strlen(unit) == 0)
	{
		res = ft_strdup(default_value);
		if (!res)
			err_ext();
	}
	else
	{
		res = ft_strdup(unit);
		if (!res)
			err_ext();
	}
	return (res);
}

char	**get_colon_units(const char *str, const char *default_value)
{
	char	**res;
	size_t	index;
	char	*copied_str;
	char	*unit_start;
	char	*unit_end;

	index = 0;
	res = allocate_colon_unit_parent(str);
	copied_str = ft_strdup(str);
	if (!copied_str)
		err_ext();
	unit_start = copied_str;
	unit_end = ft_strchr(unit_start, ':');
	while (unit_end)
	{
		*unit_end = '\0';
		res[index] = strdup_colon_unit(unit_start, default_value);
		unit_start = unit_end + 1;
		unit_end = ft_strchr(unit_start, ':');
		index++;
	}
	res[index] = strdup_colon_unit(unit_start, default_value);
	ft_safe_free_char(&copied_str);
	return (res);
}

t_bool	try_change_dir(const char *destination)
{
	char		*path;
	t_bool		is_canon_path;
	int			res;

	path = set_cd_path(destination, &is_canon_path);
	res = change_dir_process(path, destination, is_canon_path);
	ft_safe_free_char(&path);
	if (res == 0)
		return (1);
	return (0);
}

char	*try_splitted_env_path(char **split_env, const char *dest)
{
	size_t	index;
	char	*joined_dest;

	index = 0;
	joined_dest = NULL;
	while (split_env[index])
	{
		if (ft_strlen(split_env[index]) == 0)
		{
			joined_dest = ft_strdup(dest);
			if (!joined_dest)
				err_ext();
		}
		else
			joined_dest = join_path(split_env[index], dest);
		if (try_change_dir(joined_dest))
			break ;
		ft_safe_free_char(&joined_dest);
		index++;
	}
	ft_safe_free_char(&joined_dest);
	if (split_env[index])
		return (split_env[index]);
	return (NULL);
}

t_bool	try_env_path(const char *dest)
{
	char				**split_env;
	t_bool				res;
	char				*try_chdir_res;
	extern t_statuses	g_states;

	res = 0;
	split_env = get_colon_units(get_env_data("CDPATH"), "");
	if (!split_env)
		err_ext();
	try_chdir_res = try_splitted_env_path(split_env, dest);
	if (try_chdir_res)
		res = 1;
	if (res && ft_strlen(try_chdir_res) != 0)
		printf("%s\n", g_states.pwd);
	ft_safe_free_split(&split_env);
	return (res);
}

int	ft_cd(char **args)
{
	const char			*dest;
	extern t_statuses	g_states;

	dest = set_cd_destination(args);
	if (!dest)
		return (1);
	if (needs_env_path_search(args, dest))
	{
		if (try_env_path(dest))
		{
			bind_pwd_value();
			return (0);
		}
	}
	if (try_change_dir(dest))
	{
		bind_pwd_value();
		return (0);
	}
	perror("minishell: cd:");
	perror((char *)dest);
	perror(": ");
	perror(strerror(errno));
	return (1);
}