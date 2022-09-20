/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 03:36:45 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cwd_path(char *caller)
{
	char	*cwd;

	cwd = getcwd(0, 0);
	if (!cwd)
	{
		ft_putstr_fd("%s: error retrieving current directory:", 2);
		ft_putstr_fd(caller, 2);
		perror(" getcwd: cannot access parent directories: ");
	}
	return (cwd);
}

char	*join_path(const char *prev, const char *next)
{
	char	*tmp;
	char	*res;
	size_t	index;

	if (!prev || !next)
		return (0);
	tmp = 0;
	res = 0;
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

	new_pwd = 0;
	if (is_abs_path)
	{
		if (is_canon_path == 0)
			new_pwd = get_cwd_path("cd");
		if (is_canon_path || new_pwd == 0)
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

int	chng_dir_process(char *cd_path, const char *arg, t_bool is_orig_path)
{
	int					res;
	int					err;
	extern t_statuses	g_states;

	res = chdir(cd_path);
	if (res == 0)
	{
		ft_safe_free_char(&g_states.pwd);
		g_states.pwd = get_new_pwd(cd_path, is_orig_path, 1);
		return (res);
	}
	err = errno;
	res = chdir(arg);
	if (res == 0)
	{
		ft_safe_free_char(&g_states.pwd);
		g_states.pwd = get_new_pwd(cd_path, is_orig_path, 0);
		return (res);
	}
	errno = err;
	res = EXIT_FAILURE;
	return (res);
}
