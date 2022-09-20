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

const char	*set_cd_destination(char **args)
{
	t_environments	*home_env;
	size_t			index;

	index = 0;
	while (args[index])
		index++;
	if (index == 1)
	{
		home_env = get_env("HOME");
		if (!home_env)
		{
			perror("minishell: cd: HOME is not set");
			return (0);
		}
		if (!home_env->value)
			return ("");
		return (home_env->value);
	}
	return (args[1]);
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
