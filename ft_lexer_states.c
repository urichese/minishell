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

void	quote_state(t_tokeniser *toker, t_char_types type, char *str)
{
	(void)type;
	toker->token->data[toker->tok_i++] = str[toker->str_i];
	if (str[toker->str_i] == _quote)
	{
		toker->state = in_ordinarized;
		if (toker->esc_flag == 1)
			toker->tok_i -= 1;
	}
}

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

void	general_sep_process(t_tokeniser *toker, t_char_types type, char *str)
{
	if (is_io_number_token(toker, type))
		toker->token->type = _io;
	tokeniser_add_new_token(toker);
	if (type != _space && type != _tab)
	{
		toker->token->data[toker->tok_i++] = str[toker->str_i];
		if (str[toker->str_i + 1] == str[toker->str_i])
			detect_char_types(&type, toker, str);
		toker->token->type = type;
		tokeniser_add_new_token(toker);
	}
}

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
