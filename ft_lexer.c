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
	const char	commands[] = {
			'|', '\'', '\"', ' ', ';', '\\', '>', '<', '\t', '\0'};

	command_count = 10;
	while (command_count--)
	{
		if (commands[command_count] == c)
			return (commands[command_count]);
	}
	return (_ordinary);
}

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
