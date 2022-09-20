/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:48:30 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*get_env_data(char *name)
{
	t_environments		*now;
	extern t_statuses	g_states;

	if (!g_states.env || !name)
		return (0);
	now = g_states.env;
	while (now)
	{
		if (strcmp(now->name, name) == 0)
		{
			if (!now->value)
				return ("");
			else
				return (now->value);
		}
		now = now->next;
	}
	return ("");
}

char	*dup_env_value(char *name)
{
	char				*res;
	extern t_statuses	g_states;

	if (strcmp("?", name) == 0)
	{
		res = ft_itoa(g_states.last_status);
		if (!res)
			err_ext();
	}
	else
	{
		res = strdup(get_env_data(name));
		if (!res)
			err_ext();
	}
	return (res);
}

size_t	calc_escaped_value_len(const char *str, const char *esc)
{
	size_t	index;
	size_t	res;

	index = 0;
	res = 0;
	while (str[index] != 0)
	{
		if (strchr(esc, str[index]) != 0)
			res++;
		res++;
		index++;
	}
	return (res);
}

void	copy_escaped_value(const char *src, const char *esc, char *dest)
{
	size_t	res_index;
	size_t	index;

	index = 0;
	res_index = 0;
	while (src[index] != 0)
	{
		if (strchr(esc, src[index]) != 0)
		{
			dest[res_index] = '\\';
			res_index++;
		}
		dest[res_index] = src[index];
		res_index++;
		index++;
	}
	dest[res_index] = '\0';
}

void	ft_expander(t_token **tkns)
{
	t_token	*vars[4];
	char	*expanded_str;

	if (!tkns || !*tkns)
		return ;
	vars[1] = 0;
	vars[3] = 0;
	vars[0] = *tkns;
	while (vars[0] != 0)
	{
		expanded_str = expand_env_var(vars[0]->data);
		vars[2] = ft_lexer(expanded_str, 1);
		free(expanded_str);
		if (vars[3] == 0)
			vars[3] = vars[2];
		token_join(vars[1], vars[2]);
		vars[1] = find_last_token(vars[3]);
		vars[0] = vars[0]->next;
	}
	free_tokens(tkns);
	*tkns = vars[3];
}
