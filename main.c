/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 03:14:45 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_statuses	g_states;

void	pwd_value_init(t_environments *pwd_env)
{
	char				*cwd;
	extern t_statuses	g_states;

	cwd = getcwd(0, 0);
	if (!cwd)
		err_ext();
	if (!pwd_env->value || !is_same_dir(pwd_env->value, cwd))
	{
		pwd_env->value = ft_strdup(cwd);
		if (!pwd_env->value)
			err_ext();
	}
	g_states.pwd = ft_strdup(pwd_env->value);
	if (!g_states.pwd)
		err_ext();
	free(cwd);
}

void	initialize_pwd(void)
{
	t_environments		*pwd_env;
	extern t_statuses	g_states;

	pwd_env = get_env("PWD");
	if (!pwd_env)
	{
		pwd_env = malloc(sizeof(t_environments));
		if (!pwd_env)
			err_ext();
		pwd_env->name = ft_strdup("PWD");
		if (!pwd_env->name)
			err_ext();
		pwd_env->value = 0;
		pwd_env->next = 0;
		pwd_env->is_environment = 1;
		add_env(&g_states.env, pwd_env);
	}
	pwd_value_init(pwd_env);
}

void	catch_signals(int signal)
{
	int	previous_error;

	previous_error = errno;
	if (signal == 2)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("  \b\b\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signal == 3)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("  \b\b");
		rl_redisplay();
	}
	errno = previous_error;
}

void	initialize_shlvl(void)
{
	t_environments		*shlvl;
	extern t_statuses	g_states;

	shlvl = get_env("SHLVL");
	if (!shlvl)
	{
		shlvl = malloc(sizeof(t_environments));
		if (!shlvl)
			err_ext();
		shlvl->name = ft_strdup("SHLVL");
		shlvl->value = ft_strdup("1");
		if (!shlvl->name || shlvl->value)
			err_ext();
		shlvl->next = NULL;
		shlvl->is_environment = 1;
		add_env(&g_states.env, shlvl);
		return ;
	}
	else
	{
		calc_shlvl(&(shlvl->value));
		if (!(shlvl->value))
			err_ext();
	}
}

int	main(void)
{
	char	*line;

	fill_envs();
	initialize_pwd();
	initialize_shlvl();
	initialize_old_pwd();
	while (!g_states.is_exited)
	{
		g_states.is_exited = 0;
		set_signal_handler(catch_signals);
		line = readline("minishell\u263A> ");
		if (line && ft_strlen(line) > 0)
			add_history(line);
		ft_exec(line);
		smart_free((void **) &line);
	}
	return (g_states.last_status);
}
