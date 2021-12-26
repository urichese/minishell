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

void	general_state(t_tokeniser *toker, t_char_types	type, char *str)
{
	if (type == _quote || type == _double_quote
		|| type == _escape || type == _ordinary)
	{
		general_esc_process(toker, type, str);
		if (type == _quote)
		{
			toker->state = in_quoted;
			toker->is_quoted = 1;
			if (toker->esc_flag)
				toker->tok_i -= 1;
		}
		else if (type == _double_quote)
		{
			toker->state = in_dquoted;
			toker->is_quoted = 1;
			if (toker->esc_flag)
				toker->tok_i -= 1;
		}
		else
			toker->state = in_ordinarized;
		toker->token->type = _token;
	}
	else
		general_sep_process(toker, type, str);
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

t_token	*ft_lexer(char *str, t_bool esc_flag)
{
	t_tokeniser		toker;
	t_char_types	type;

	if (!str)
		return (0);
	tokeniser_init(&toker, str, esc_flag);
	while (str[toker.str_i] != '\0' && toker.tokens_start)
	{
		type = detect_type_of_token(str[toker.str_i]);
		if (toker.state == in_ordinarized)
			general_state(&toker, type, str);
		else if (toker.state == in_quoted)
			quote_state(&toker, type, str);
		else if (toker.state == in_dquoted)
			d_quote_state(&toker, type, str);
		toker.str_i++;
	}
	close_token_list(&toker);
	return (toker.tokens_start);
}
