/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:53:11 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(t_token *token)
{
	extern t_statuses	g_states;

	perror("minishell: syntax error: unexpected token `");
	if (token)
		perror(token->data);
	else
		perror("newline");
	g_states.last_status = 258;
}

t_bool	set_redirect_type(t_token *tkn, t_redirect *redirect)
{
	if (tkn->type == _lesser)
		redirect->type = _red_input;
	else if (tkn->type == _greater)
		redirect->type = _red_output;
	else if (tkn->type == _double_greater)
		redirect->type = _red_d_output;
	else
		return (0);
	set_redirect_fd(redirect);
	return (1);
}
