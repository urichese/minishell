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

char	*create_expand_str(const char *str, t_state	state,
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

void	merge_strings(t_expander *exper,
			char *vars[4], char *env_value, size_t after_var_index)
{
	vars[1] = create_expand_str(env_value, exper->state, 0);
	if (!vars[1])
		err_ext();
	vars[2] = ft_strjoin(exper->str, vars[1]);
	if (!vars[2])
		err_ext();
	vars[3] = ft_strjoin(vars[2], &exper->str[after_var_index]);
	if (!vars[3])
		err_ext();
	exper->str_i = strlen(vars[2]) - 1;
}

void	expand_var_in_str(t_expander *exper)
{
	char	*vars[4];
	char	*env_value;
	size_t	after_var_index;

	vars[0] = extract_var_name(&exper->str[exper->str_i + 1]);
	if (!vars[0])
		err_ext();
	if (ft_strlen(vars[0]) == 0)
	{
		ft_safe_free_char(&vars[0]);
		return ;
	}
	exper->str[exper->str_i] = '\0';
	env_value = dup_env_value(vars[0]);
	after_var_index = exper->str_i + ft_strlen(vars[0]) + 1;
	merge_strings(exper, vars, env_value, after_var_index);
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
			&& ft_strchr("\\\'\"$", exper.str[exper.str_i + 1]) != 0)
			exper.str_i++;
		else if (exper.str[exper.str_i] == '$'
			&& (exper.state == in_ordinarized || exper.state == in_dquoted))
			expand_var_in_str(&exper);
		exper.str_i++;
	}
	return (exper.str);
}
