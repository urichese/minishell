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

static t_bool	parse_io_redirect(t_token **tkns, t_node *command_node)
{
	t_redirect	*redirect;

	redirect = create_redirect();
	if ((*tkns)->type == _io)
	{
		if (ft_atoi_with_limit((*tkns)->data, &redirect->fd_io) == 0)
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

static t_bool	parse_command(t_pre_command	**cmd,
					t_node **node, t_token **tokens)
{
	if (!*tokens)
		return (0);
	*node = create_command_node(cmd);
	*cmd = (*node)->command;
	while (*tokens)
	{
		if ((*tokens)->type == _token)
			set_command_args((*node)->command, tokens);
		else if (is_redirect_token(*tokens))
		{
			if (parse_io_redirect(tokens, *node) == 0)
			{
				free_names(node);
				return (0);
			}
		}
		else
			break ;
	}
	if (!(*node)->command->args && !(*node)->command->redirects)
	{
		free_names(node);
		return (0);
	}
	return (1);
}

static t_bool	parse_pipeline(
		t_pre_command **last_command, t_node **node, t_token **tokens)
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

static t_bool	parse_separator(t_node **nodes, t_token **tokens)
{
	t_node			*child;
	t_pre_command	*last_command;

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

t_bool	ft_parser(t_node **nodes, t_token **tokens)
{
	t_bool	result;

	*nodes = 0;
	result = parse_separator(nodes, tokens);
	return (result);
}
