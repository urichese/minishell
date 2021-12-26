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

void	token_join(t_token *prev_token, t_token *next_token)
{
	if (!prev_token || !next_token || prev_token == next_token)
		return ;
	prev_token->next = next_token;
	next_token->prev = prev_token;
}

t_token	*find_last_token(t_token *tkns)
{
	t_token	*now;

	if (!tkns)
		return (0);
	now = tkns;
	while (now->next != 0)
		now = now->next;
	return (now);
}

t_state	judge_token_state(t_state	state, t_char_types type)
{
	if (state == in_ordinarized)
	{
		if (type == _double_quote)
			return (in_dquoted);
		if (type == _quote)
			return (in_quoted);
	}
	else if (state == in_dquoted && type != _double_quote)
		return (in_dquoted);
	else if (state == in_quoted && type != _quote)
		return (in_quoted);
	return (in_ordinarized);
}

char	*extract_var_name(char *str)
{
	size_t	var_len;
	char	*res;

	if (*str == '?')
		return (strdup("?"));
	var_len = 0;
	if (ft_isdigit(*str))
	{
		res = strdup("");
		if (!res)
			err_ext();
		return (res);
	}
	while (ft_isalnum(str[var_len]) || str[var_len] == '_')
		var_len++;
	res = malloc(sizeof(char) * var_len + 1);
	if (!res)
		err_ext();
	strlcpy(res, str, var_len + 1);
	return (res);
}
