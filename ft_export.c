/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_identifier(char *name)
{
	size_t	i;

	i = 0;
	if (name[i] != '_' && !ft_isalpha(name[i]))
		return (0);
	i++;
	while (name[i])
	{
		if (name[i] != '_' && !ft_isalnum(name[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	separate_arg(char *arg, char **sep, char **value,
								t_bool *append_flag)
{
	*sep = ft_strchr(arg, '=');
	if (*sep)
	{
		**sep = '\0';
		if (*sep != arg && *(*sep - 1) == '+')
		{
			*(*sep - 1) = '\0';
			*append_flag = 1;
		}
		*value = *sep + 1;
	}
	else
		*value = 0;
}

static void	restore_arg(char *sep, t_bool append_flag)
{
	if (sep)
	{
		*sep = '=';
		if (append_flag == 1)
			*(sep - 1) = '+';
	}
}

static int	set_envs(char **args)
{
	size_t	i;
	char	*sep;
	char	*value;
	t_bool	append_flag;
	int		ret;

	ret = 0;
	i = 1;
	value = 0;
	while (args[i])
	{
		append_flag = 0;
		separate_arg(args[i], &sep, &value, &append_flag);
		if (is_identifier(args[i]))
			update_env_value(args[i], value, 1, append_flag);
		else
		{
			restore_arg(sep, append_flag);
			ft_putstr_fd("export ", 2);
			perror(args[i]);
			ret = 1;
		}
		i++;
	}
	return (ret);
}

int	ft_export(char **args)
{
	if (args[1])
		return (set_envs(args));
	else
		return (print_envs());
}
