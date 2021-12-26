/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bind_pwd_value(void)
{
	extern t_statuses	g_states;

	update_env_value("OLDPWD", get_env_data("PWD"), 0, 0);
	update_env_value("PWD", g_states.pwd, 0, 0);
}

int	ft_pwd(void)
{
	extern t_statuses	g_states;

	printf("%s\n", g_states.pwd);
	return (0);
}
