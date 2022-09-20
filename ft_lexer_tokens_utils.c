/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:51:50 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_io_number_token(t_tokeniser *toker, t_char_types type)
{
	size_t	i;

	if (!toker || !toker->token || !toker->token->data
		|| toker->tok_i == 0
		|| (type != _greater && type != _lesser))
		return (0);
	i = toker->tok_i;
	while (i != 0 && ft_isdigit(toker->token->data[i - 1]))
		i--;
	if (i == 0)
		return (1);
	return (0);
}

void	free_tokens(t_token **token_p)
{
	t_token	*now;
	t_token	*tmp;

	if (!token_p || !*token_p)
		return ;
	now = *token_p;
	while (now)
	{
		tmp = now->next;
		del_token(&now);
		now = tmp;
	}
	*token_p = 0;
}

void	print_token_error(t_state state)
{
	extern t_statuses	g_states;

	if (state == in_dquoted)
		perror("\" is not closed");
	if (state == in_quoted)
		perror("' is not closed");
	g_states.last_status = 2;
}

void	close_token_list(t_tokeniser *toker)
{
	if (!toker->tokens_start)
		return ;
	if (toker->state != in_ordinarized)
	{
		print_token_error(toker->state);
		free_tokens(&(toker->tokens_start));
		return ;
	}
	if (toker->tok_i == 0 && toker->is_quoted == 0)
	{
		if (toker->tokens_start == toker->token)
			free_tokens(&toker->tokens_start);
		else
			del_token(&toker->token);
	}
	else
		toker->token->data[toker->tok_i] = '\0';
}
