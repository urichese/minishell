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

void	ft_safe_free_split(char ***target)
{
	size_t	index;

	index = 0;
	if (!*target)
		return ;
	while ((*target)[index])
	{
		free((*target)[index]);
		(*target)[index] = 0;
		index++;
	}
	free(*target);
	*target = 0;
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

int	ft_cd(char **args)
{
	const char	*dest;

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
	ft_putstr_fd("minishell: cd: ", 2);
	perror((char *)dest);
	return (1);
}
