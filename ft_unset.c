/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_env(t_environments **envs, char *name)
{
	t_environments	*now;
	t_environments	*prev;

	prev = 0;
	now = *envs;
	while (now)
	{
		if (ft_strncmp(now->name, name, ft_strlen(name) + 1) == 0)
		{
			if (prev)
				prev->next = now->next;
			else
				*envs = now->next;
			ft_safe_free_char(&now->name);
			ft_safe_free_char(&now->value);
			free(now);
			now = 0;
			break ;
		}
		prev = now;
		now = now->next;
	}
}

int	ft_unset(char **args)
{
	extern t_statuses	g_states;
	size_t				i;
	int					return_val;

	i = 1;
	return_val = 0;
	while (args[i])
	{
		if (is_identifier(args[i]) == 1)
			del_env(&g_states.env, args[i]);
		else
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return_val = 1;
		}
		i++;
	}
	return (return_val);
}
