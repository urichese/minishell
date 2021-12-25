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

static t_environments	*env_merge(t_environments *left,
									t_environments *right, int (*cmp)())
{
	t_environments	elem;
	t_environments	*next;

	next = &elem;
	while (left != NULL && right != NULL)
	{
		if (cmp(left, right) < 0)
		{
			next->next = left;
			next = next->next;
			left = left->next;
		}
		else
		{
			next->next = right;
			next = next->next;
			right = right->next;
		}
	}
	if (left == NULL)
		next->next = right;
	else
		next->next = left;
	return (elem.next);
}

static t_environments	*env_mergesort_sub(t_environments *lst, int (*cmp)())
{
	t_environments	*left;
	t_environments	*right;
	t_environments	*right_head;

	if (lst == NULL || lst->next == NULL)
		return (lst);
	left = lst;
	right = lst->next;
	if (right != NULL)
		right = right->next;
	while (right != NULL)
	{
		left = left->next;
		right = right->next;
		if (right != NULL)
			right = right->next;
	}
	right_head = left->next;
	left->next = NULL;
	return (env_merge(env_mergesort_sub(lst, cmp),
			  env_mergesort_sub(right_head, cmp), cmp));
}

void	env_mergesort(t_environments **lst, int (*cmp)())
{
	*lst = env_mergesort_sub(*lst, cmp);
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
