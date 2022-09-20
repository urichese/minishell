/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 18:04:20 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:25:04 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	environment_free(t_environments *env)
{
	free(env->name);
	free(env->value);
	env->name = 0;
	env->value = 0;
	free(env);
}

t_environments	*get_env(const char *name)
{
	t_environments		*now;
	extern t_statuses	g_states;

	if (!g_states.env || !name)
		return (0);
	now = g_states.env;
	while (now)
	{
		if (ft_strcmp(now->name, (char *)name) == 0)
			return (now);
		now = now->next;
	}
	return (0);
}

t_environments	*copy_envs(t_environments *envs)
{
	t_environments	*res;
	t_environments	*now_env;
	t_environments	*copy_env;

	now_env = envs;
	res = 0;
	while (now_env)
	{
		copy_env = (t_environments *)malloc(sizeof(t_environments));
		if (copy_env == 0)
			err_ext();
		copy_env->name = now_env->name;
		copy_env->value = now_env->value;
		copy_env->is_environment = now_env->is_environment;
		copy_env->next = 0;
		add_env(&res, copy_env);
		now_env = now_env->next;
	}
	return (res);
}

static void	print_env(t_environments *env)
{
	char	*escaped_value;

	if (env->is_environment == 0)
		return ;
	printf("declare -x %s", env->name);
	if (env->value)
	{
		escaped_value = create_expand_str(env->value, in_dquoted, 1);
		printf("=\"%s\"", escaped_value);
		free(escaped_value);
	}
	printf("\n");
}

int	print_envs(void)
{
	extern t_statuses	g_states;
	t_environments		*envs;
	t_environments		*tmp;

	envs = copy_envs(g_states.env);
	ft_sorting(&envs);
	while (envs)
	{
		print_env(envs);
		tmp = envs->next;
		environment_free(envs);
		envs = tmp;
	}
	return (0);
}
