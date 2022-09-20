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

void	del_token(t_token **token_p)
{
	t_token	*token;

	if (!token_p || !*token_p)
		return ;
	token = *token_p;
	if (token->data)
		free(token->data);
	if (token->next)
		token->next->prev = token->prev;
	if (token->prev)
		token->prev->next = token->next;
	free(token);
	*token_p = 0;
}

t_token	*token_init(size_t len, t_token *prev)
{
	t_token	*res;

	res = malloc(sizeof(t_token));
	if (!res)
		err_ext();
	res->data = malloc(sizeof(char) * (len + 1));
	if (!res->data)
		err_ext();
	res->data[0] = '\0';
	res->type = _token;
	res->next = 0;
	res->prev = prev;
	return (res);
}

void	tokeniser_init(t_tokeniser *toker, char *str, t_bool esc_flag)
{
	size_t	len;
	t_token	*start_token;

	len = ft_strlen(str);
	start_token = token_init(len, 0);
	toker->token = start_token;
	toker->tokens_start = start_token;
	toker->state = in_ordinarized;
	toker->str_i = 0;
	toker->tok_i = 0;
	toker->str_len = len;
	toker->esc_flag = esc_flag;
	toker->is_quoted = 0;
}

t_char_types	detect_type_of_token(char c)
{
	int			command_count;
	const char	commands[] = {'|', '\'', '\"',
		' ', ';', '\\', '>', '<', '\t', '\0'};

	command_count = 10;
	while (command_count--)
	{
		if (commands[command_count] == c)
			return (commands[command_count]);
	}
	return (_ordinary);
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
