/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:53:40 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_statuses	g_states;

t_environments	*fill_envs(void)
{
	extern char		**environ;
	size_t			i;
	t_environments	*envs;
	t_environments	*now_env;

	envs = 0;
	i = 0;
	while (environ[i])
	{
		now_env = create_new_env(environ[i]);
		add_env(&envs, now_env);
		i++;
	}
	return (envs);
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

void	pwd_value_init(t_environments *pwd_env)
{
	char				*cwd;
	extern t_statuses	g_states;

	cwd = getcwd(0, 0);
	if (!cwd)
		err_ext();
	if (!pwd_env->value || !is_same_dir(pwd_env->value, cwd))
	{
		pwd_env->value = strdup(cwd);
		if (!pwd_env->value)
			err_ext();
	}
	g_states.pwd = strdup(pwd_env->value);
	if (!g_states.pwd)
		err_ext();
	free(cwd);
}

void	pwd_init(void)
{
	t_environments		*pwd_env;
	extern t_statuses	g_states;

	pwd_env = get_env("PWD");
	if (!pwd_env)
	{
		pwd_env = malloc(sizeof(t_environments));
		if (!pwd_env)
			err_ext();
		pwd_env->name = strdup("PWD");
		if (!pwd_env->name)
			err_ext();
		pwd_env->value = 0;
		pwd_env->next = 0;
		pwd_env->is_environment = 1;
		add_env(&g_states.env, pwd_env);
	}
	pwd_value_init(pwd_env);
}

int	main(void)
{
	char	*line;
	extern t_statuses	g_states;

	g_states.env = fill_envs();
	pwd_init();
//	shlvl_init();
//	old_pwd_init();
	while (!g_states.is_exited)
	{
		line = readline("minishell\u263A> ");
		add_history(line);
		g_states.is_exited = 0;
		ft_exec(line);
		smart_free((void **) &line);
	}
	return (g_states.last_status);
}

/*
 * readline, rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, kill, exit, getcwd, chdir, stat,
lstat, fstat, unlink, execve, dup, dup2, pipe,
opendir, readdir, closedir, strerror, perror,
isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr,
tcgetattr, tgetent, tgetflag, tgetnum, tgetstr,
tgoto, tputs
 */