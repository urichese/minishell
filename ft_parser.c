/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2022/01/04 03:36:29 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_redirect_token(t_token *tkn)
{
	return (tkn->type == _double_greater || tkn->type == _greater
		|| tkn->type == _io || tkn->type == _double_lesser
		|| tkn->type == _lesser);
}

t_bool	parse_io_redirect(t_token **tkns, t_node *command_node)
{
	t_redirect	*redirect;

	redirect = create_redirect();
	if ((*tkns)->type == _io)
	{
		if (ft_atoi_by_ref((*tkns)->data, &redirect->fd_io) == 0)
			redirect->fd_io = -2;
		*tkns = (*tkns)->next;
	}
	if (set_redirect_type(*tkns, redirect) == 0)
	{
		del_redirect_list(&redirect);
		return (0);
	}
	*tkns = (*tkns)->next;
	if (!*tkns || (*tkns)->type != _token)
	{
		del_redirect_list(&redirect);
		return (0);
	}
	add_copied_token(&redirect->filename, *tkns);
	add_redirect(&command_node->command->redirects, redirect);
	*tkns = (*tkns)->next;
	return (1);
}

t_bool	parse_pipeline(t_cmd **last_command,
		t_node **node, t_token **tokens)
{
	t_node	*child;

	if (parse_command(last_command, node, tokens) == 0)
		return (0);
	while (*tokens)
	{
		if (has_token_type(tokens, _pipe))
		{
			if (parse_command(last_command, &child, tokens) == 0)
				return (0);
			*node = add_parent_node(_node_type_pipe, *node, child);
		}
		else
			break ;
	}
	return (1);
}

t_bool	ft_parser(t_node **nodes, t_token **tokens)
{
	t_node	*child;
	t_cmd	*last_command;

	last_command = 0;
	if (*tokens)
	{
		if (parse_pipeline(&last_command, nodes, tokens) == 0)
			return (0);
	}
	while (*tokens)
	{
		if (has_token_type(tokens, _semicolon) && *tokens)
		{
			last_command = 0;
			if (parse_pipeline(&last_command, &child, tokens) == 0)
				return (0);
			*nodes = add_parent_node(_node_type_semicol, *nodes, child);
		}
		else
			break ;
	}
	if (*tokens)
		return (0);
	return (1);
}

void	print_syntax_error(t_token *token)
{
	extern t_statuses	g_states;

	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token)
		ft_putstr_fd(token->data, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_printstr_nl("'", 2);
	g_states.last_status = 258;
}
