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

t_environments	*create_new_env(char *env_str)
{
	t_environments	*env;
	char			*sep;

	env = malloc(sizeof(t_environments));
	if (!env)
		err_ext();
	sep = ft_strchr(env_str, '=');
	if (!sep)
	{
		env->name = strdup(env_str);
		if (!env->name)
			err_ext();
		env->value = NULL;
	}
	else
	{
		env->name = ft_substr(env_str, 0, sep - env_str);
		env->value = strdup(sep + 1);
		if (!env->name || !env->value)
			err_ext();
	}
	env->is_environment = 1;
	env->next = 0;
	return (env);
}

void	free_env(t_environments *env)
{
	free(env->name);
	free(env->value);
	env->name = NULL;
	env->value = NULL;
	free(env);
}

t_environments	*get_last_env(t_environments *envs)
{
	t_environments	*target;

	if (!envs)
		return (NULL);
	target = envs;
	while (target->next)
		target = target->next;
	return (target);
}

void	add_env(t_environments **envs, t_environments *new_env)
{
	if (!new_env || !envs)
		return ;
	if (!*envs)
		*envs = new_env;
	else
	{
		get_last_env(*envs)->next = new_env;
		new_env->next = NULL;
	}
}
