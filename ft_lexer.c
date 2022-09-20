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

void	detect_char_types(t_char_types	*type, t_tokeniser *toker, char *str)
{
	if (*type == _greater)
	{
		toker->token->data[toker->tok_i++] = str[++toker->str_i];
		*type = _double_greater;
	}
	else if (*type == _lesser)
	{
		toker->token->data[toker->tok_i++] = str[++toker->str_i];
		*type = _double_lesser;
	}
	else if (*type == _semicolon)
	{
		toker->token->data[toker->tok_i++] = str[++toker->str_i];
		*type = _double_semicolon;
	}
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
