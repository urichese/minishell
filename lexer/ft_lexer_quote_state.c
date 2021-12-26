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

void	d_quote_state(t_tokeniser *toker, t_char_types type, char *str)
{
	if (type == _escape && str[toker->str_i + 1] != '\0'
		&& ft_strchr("\"\\$", str[toker->str_i + 1]) != 0)
	{
		if (toker->esc_flag)
			toker->token->data[toker->tok_i++] = str[++toker->str_i];
		else
		{
			toker->token->data[toker->tok_i++] = str[toker->str_i++];
			toker->token->data[toker->tok_i++] = str[toker->str_i];
		}
	}
	else
	{
		toker->token->data[toker->tok_i++] = str[toker->str_i];
		if (str[toker->str_i] == _double_quote)
		{
			toker->state = in_ordinarized;
			if (toker->esc_flag == 1)
				toker->tok_i -= 1;
		}
	}
}

void	general_esc_process(t_tokeniser *toker, t_char_types type, char *str)
{
	if (type == _escape && str[toker->str_i + 1] != '\0')
	{
		if (toker->esc_flag)
			toker->token->data[toker->tok_i++] = str[++toker->str_i];
		else
		{
			toker->token->data[toker->tok_i++] = str[toker->str_i++];
			toker->token->data[toker->tok_i++] = str[toker->str_i];
		}
	}
	else
	{
		toker->token->data[toker->tok_i++] = str[toker->str_i];
	}
}

void	tokeniser_add_new_token(t_tokeniser *toker)
{
	t_token	*tmp_token;

	if (toker->tok_i > 0 || (toker->is_quoted))
	{
		toker->token->data[toker->tok_i] = '\0';
		tmp_token = token_init(toker->str_len - toker->str_i, toker->token);
		toker->token->next = tmp_token;
		toker->token = tmp_token;
		toker->tok_i = 0;
		toker->is_quoted = 0;
	}
}

t_bool	is_io_number_token(t_tokeniser *toker, t_char_types type)
{
	size_t	i;

	if (!toker || !toker->token || !toker->token->data
		|| toker->tok_i == 0
		|| (type != _greater && type != _lesser))
	{
		return (0);
	}
	i = toker->tok_i;
	while (i != 0 && ft_isdigit(toker->token->data[i - 1]))
		i--;
	if (i == 0)
		return (1);
	return (0);
}

void	general_sep_process(t_tokeniser *toker, t_char_types	type, char *str)
{
	if (is_io_number_token(toker, type))
		toker->token->type = _io;
	tokeniser_add_new_token(toker);
	if (type != _space && type != _tab)
	{
		toker->token->data[toker->tok_i++] = str[toker->str_i];
		if (str[toker->str_i + 1] == str[toker->str_i])
		{
			if (type == _greater)
			{
				toker->token->data[toker->tok_i++] = str[++toker->str_i];
				type = _double_greater;
			}
			else if (type == _semicolon)
			{
				toker->token->data[toker->tok_i++] = str[++toker->str_i];
				type = _double_semicolon;
			}
		}
		toker->token->type = type;
		tokeniser_add_new_token(toker);
	}
}
