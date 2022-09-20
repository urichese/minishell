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

t_bool	needs_env_path_search(char **args, const char *dest)
{
	if (args[1] == 0 || args[1][0] == '/')
		return (0);
	if (ft_strcmp((char *)dest, ".") == 0
		|| ft_strcmp((char *)dest, "..") == 0
		|| ft_strncmp((char *)dest, "./", 2) == 0
		|| ft_strncmp((char *)dest, "../", 3) == 0)
		return (0);
	return (1);
}

t_bool	try_change_dir(const char *destination)
{
	char		*path;
	t_bool		is_canon_path;
	int			res;

	path = set_cd_path(destination, &is_canon_path);
	res = chng_dir_process(path, destination, is_canon_path);
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
	joined_dest = 0;
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
	return (0);
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
