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

void	expander_init(t_expander *exper, char *input)
{
	exper->str = strdup(input);
	if (exper->str == 0)
		err_ext();
	exper->str_i = 0;
	exper->state = in_ordinarized;
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

void	ft_safe_free_char(char **target)
{
	free(*target);
	*target = 0;
}

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
