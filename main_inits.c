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

void	print_shell_level_error(int num)
{
	char	*str_num;

	str_num = ft_itoa(num);
	if (!str_num)
		err_ext();
	ft_putstr_fd("shell level (", 2);
	ft_putstr_fd(str_num, 2);
	perror(") too high, resetting to 1");
	smart_free((void **) &str_num);
}

t_bool	is_digit_str(char *str)
{
	size_t	index;
	t_bool	has_digit;

	index = 0;
	has_digit = 0;
	while (ft_isspace(str[index]))
		index++;
	if (str[index] == '+' || str[index] == '-')
		index++;
	while (str[index])
	{
		if (ft_isdigit(str[index]))
			has_digit = 1;
		else
			break ;
		index++;
	}
	while (str[index] == ' ' || str[index] == '\t')
		index++;
	if (str[index] == '\0' && has_digit == 1)
		return (1);
	else
		return (0);
}

void	calc_shlvl(char **shlvl)
{
	char	*res;
	int		num;

	num = ft_atoi_over(*shlvl);
	if (!is_digit_str(*shlvl))
		num = 0;
	num++;
	if (num == 1000)
		res = ft_strdup("");
	else if (num < 1)
		res = ft_strdup("0");
	else if (0 < num && num < 1000)
		res = ft_itoa(num);
	else
	{
		print_shell_level_error(num);
		res = ft_strdup("1");
	}
	ft_safe_free_char(shlvl);
	*shlvl = res;
}

void	initialize_old_pwd(void)
{
	t_environments		*old_pwd_env;
	extern t_statuses	g_states;

	old_pwd_env = get_env("OLDPWD");
	if (!old_pwd_env)
	{
		old_pwd_env = malloc(sizeof(t_environments));
		if (!old_pwd_env)
			err_ext();
		old_pwd_env->name = ft_strdup("OLDPWD");
		if (!old_pwd_env)
			err_ext();
		old_pwd_env->value = 0;
		old_pwd_env->next = 0;
		old_pwd_env->is_environment = 1;
		add_env(&g_states.env, old_pwd_env);
	}
	ft_safe_free_char(&(old_pwd_env->value));
}
