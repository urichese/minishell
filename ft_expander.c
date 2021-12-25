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

typedef	struct	s_expander
{
	size_t			str_i;
	t_state			state;
	t_char_types	type;
	char			*str;
}				t_expander;

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

void	ft_safe_free_char(char **target)
{
	free(*target);
	*target = 0;
}

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

char	*create_expanded_str(const char *str, t_state	state,
							 t_bool is_env)
{
	char	*esc_chars;
	char	*res;

	esc_chars = "\"\\$";
	if (state == in_ordinarized)
		esc_chars = "\'\"\\$|;><";
	if (is_env == 1)
		esc_chars = "\"\\$`";
	res = malloc(sizeof(char *) * (calc_escaped_value_len(str, esc_chars) + 1));
	if (!res)
		err_ext();
	copy_escaped_value(str, esc_chars, res);
	return (res);
}

void	expand_var_in_str(t_expander *exper)
{
	char			*vars[4];
	char			*env_value;
	size_t			after_var_index;

	vars[0] = extract_var_name(&exper->str[exper->str_i + 1]);
	if (!vars[0])
		err_ext();
	if (strlen(vars[0]) == 0)
	{
		ft_safe_free_char(&vars[0]);
		return ;
	}
	exper->str[exper->str_i] = '\0';
	env_value = dup_env_value(vars[0]);
	after_var_index = exper->str_i + strlen(vars[0]) + 1;
	vars[1] = create_expanded_str(env_value, exper->state, 0);
	if (!vars[1])
		err_ext();
	vars[2] = ft_strjoin(exper->str, vars[1]);
	if (!vars[2])
		err_ext();
	vars[3] = ft_strjoin(vars[2], &exper->str[after_var_index]);
	if (!vars[3])
		err_ext();
	exper->str_i = strlen(vars[2]) - 1;
	free(vars[1]);
	free(vars[0]);
	free(vars[2]);
	free(env_value);
	free(exper->str);
	exper->str = vars[3];
}

char	*expand_env_var(char *input)
{
	t_expander	exper;

	if (!input)
		return (0);
	expander_init(&exper, input);
	while (exper.str[exper.str_i] != '\0')
	{
		exper.type = detect_type_of_token(exper.str[exper.str_i]);
		exper.state = judge_token_state(exper.state, exper.type);
		if (exper.type == _escape && exper.str[exper.str_i + 1] != '\0'
			&& strchr("\\\'\"$", exper.str[exper.str_i + 1]) != 0)
			exper.str_i++;
		else if (exper.str[exper.str_i] == '$'
			&& (exper.state == in_ordinarized || exper.state == in_dquoted))
			expand_var_in_str(&exper);
		exper.str_i++;
	}
	return (exper.str);
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

void	ft_expander(t_token **tkns)
{
	t_token	*vars[4];
	char	*expanded_str;

	if (!tkns || !*tkns)
		return ;
	vars[1] = NULL;
	vars[3] = NULL;
	vars[0] = *tkns;
	while (vars[0] != NULL)
	{
		expanded_str = expand_env_var(vars[0]->data);
		vars[2] = ft_lexer(expanded_str, 1);
		free(expanded_str);
		if (vars[3] == NULL)
			vars[3] = vars[2];
		token_join(vars[1], vars[2]);
		vars[1] = find_last_token(vars[3]);
		vars[0] = vars[0]->next;
	}
	free_tokens(tkns);
	*tkns = vars[3];
}
