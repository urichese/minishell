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

static void	set_env_value(t_environments *env,
				const char *new_value, t_bool append_flag)
{
	char	*old_value;

	old_value = env->value;
	if (append_flag == 1)
	{
		if (old_value || new_value)
		{
			env->value = ft_strjoin(old_value, new_value);
			if (!env->value)
				err_ext();
		}
		else
			env->value = NULL;
	}
	else
	{
		if (new_value)
		{
			env->value = strdup(new_value);
			if (!env->value)
				err_ext();
		}
		else
			env->value = NULL;
	}
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
		env = create_new_env((char *)env_name);
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

static void	print_env(t_environments *env)
{
	char	*escaped_value;

	if (env->is_environment == 0)
		return ;
	printf("declare -x %s", env->name);
	if (env->value)
	{
		escaped_value = create_expanded_str(env->value, in_dquoted, 1);
		printf("=\"%s\"", escaped_value);
		free(escaped_value);
	}
	printf("\n");
}

static int	compare_env(t_environments *left, t_environments *right)
{
	return (ft_strcmp(left->name, right->name));
}
