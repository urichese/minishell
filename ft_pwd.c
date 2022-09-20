/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_envs(void)
{
	extern char			**environ;
	size_t				i;
	t_environments		*now_env;
	extern t_statuses	g_states;

	g_states.env = 0;
	i = 0;
	while (environ[i])
	{
		now_env = create_environment(environ[i]);
		add_env(&g_states.env, now_env);
		i++;
	}
}

t_bool	is_same_dir(const char *dir_1, const char *dir_2)
{
	struct stat	stat1;
	struct stat	stat2;

	if (!dir_1 || !dir_2)
		return (0);
	if (stat(dir_1, &stat1) < 0 || stat(dir_2, &stat2) < 0)
		return (0);
	if (stat1.st_ino == stat2.st_ino)
		return (1);
	return (0);
}

void	bind_pwd_value(void)
{
	extern t_statuses	g_states;

	update_env_value("OLDPWD", get_env_data("PWD"), 0, 0);
	update_env_value("PWD", g_states.pwd, 0, 0);
}

int	ft_pwd(void)
{
	extern t_statuses	g_states;

	printf("%s\n", g_states.pwd);
	return (0);
}
