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

#include "../minishell.h"

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

static int	compare_env(t_environments *left, t_environments *right)
{
	return (ft_strcmp(left->name, right->name));
}

int	print_envs(void)
{
	extern t_statuses	g_states;
	t_environments		*envs;
	t_environments		*tmp;

	envs = copy_envs(g_states.env);
	env_mergesort(&envs, compare_env);
	while (envs)
	{
		print_env(envs);
		tmp = envs->next;
		free(envs);
		envs = tmp;
	}
	return (0);
}
