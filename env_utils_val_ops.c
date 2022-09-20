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

t_environments	*create_environment(char *env_str)
{
	t_environments	*env;
	char			*sep;

	env = malloc(sizeof(t_environments));
	if (!env)
		err_ext();
	sep = ft_strchr(env_str, '=');
	if (!sep)
	{
		env->name = ft_strdup(env_str);
		if (!env->name)
			err_ext();
		env->value = 0;
	}
	else
	{
		env->name = ft_substr(env_str, 0, sep - env_str);
		env->value = ft_strdup(sep + 1);
		if (!env->name || !env->value)
			err_ext();
	}
	env->is_environment = 1;
	env->next = 0;
	return (env);
}

static void	set_env_value(t_environments *env,
			const char *new_value, t_bool append_flag)
{
	char	*old_value;

	old_value = env->value;
	if (append_flag == 1 && (new_value || old_value))
	{
		env->value = ft_strjoin(old_value, new_value);
		if (!env->value)
			err_ext();
	}
	else if (new_value)
	{
		env->value = ft_strdup(new_value);
		if (!env->value)
			err_ext();
	}
	else
		env->value = 0;
	ft_safe_free_char(&old_value);
}

void	update_env_value(const char *env_name, const char *new_value,
		t_bool is_env_var, t_bool append_flag)
{
	extern t_statuses	g_states;
	t_environments		*env;

	if (!env_name)
		return ;
	env = get_env(env_name);
	if (!env)
	{
		env = create_environment((char *) env_name);
		env->is_environment = is_env_var;
		add_env(&g_states.env, env);
	}
	else
	{
		if (env->is_environment == 0)
			env->is_environment = is_env_var;
		if (!new_value)
			return ;
	}
	set_env_value(env, new_value, append_flag);
}

t_environments	*get_last_env(t_environments *envs)
{
	t_environments	*target;

	if (!envs)
		return (0);
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
		new_env->next = 0;
	}
}
