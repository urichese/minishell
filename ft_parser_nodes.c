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

t_node	*create_command_node(t_cmd **cmd_for_node)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		err_ext();
	node->command = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node->command)
		err_ext();
	node->type = _node_type_cmd;
	node->left = 0;
	node->right = 0;
	node->command->pid = -1;
	node->command->next = 0;
	node->command->args = 0;
	node->command->redirects = 0;
	if (*cmd_for_node)
		(*cmd_for_node)->next = node->command;
	(*cmd_for_node) = node->command;
	return (node);
}

t_bool	parse_command(t_cmd	**cmd,
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

t_node	*add_parent_node(t_node_type type, t_node *left, t_node *right)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		err_ext();
	node->right = right;
	node->command = 0;
	node->type = type;
	node->left = left;
	return (node);
}
