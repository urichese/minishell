/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 03:27:34 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_environ_i(char **env_i, t_environments *envs)
{
	char	*orig_env_i;

	*env_i = ft_strjoin(envs->name, "=");
	if (!*env_i)
		err_ext();
	orig_env_i = *env_i;
	*env_i = ft_strjoin(*env_i, envs->value);
	if (!*env_i)
		err_ext();
	smart_free((void **) &orig_env_i);
}

static void	print_env(t_environments *env)
{
	if (env->value == 0 || env->is_environment == 0)
		return ;
	printf("%s=%s\n", env->name, env->value);
}

int	ft_env(void)
{
	extern t_statuses	g_states;
	t_environments		*env;

	env = g_states.env;
	while (env)
	{
		print_env(env);
		env = env->next;
	}
	return (0);
}
