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

t_node	*create_command_node(t_pre_command **cmd_for_node)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		err_ext();
	node->command = (t_pre_command *)malloc(sizeof(t_pre_command));
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

static t_token	*copy_token(t_token *token)
{
	t_token	*new;
	size_t	data_len;

	data_len = ft_strlen(token->data);
	new = token_init(data_len, 0);
	ft_strlcpy(new->data, token->data, data_len + 1);
	new->type = token->type;
	return (new);
}

void	add_copied_token(t_token **list, t_token *original_token)
{
	t_token	*now;
	t_token	*copied_token;

	copied_token = copy_token(original_token);
	if (!*list)
		*list = copied_token;
	else
	{
		now = *list;
		while (now->next)
			now = now->next;
		now->next = copied_token;
		copied_token->prev = now->next;
	}
}

void	set_command_args(t_pre_command *command, t_token **tokens)
{
	while (*tokens && (*tokens)->type == _token)
	{
		add_copied_token(&command->args, *tokens);
		*tokens = (*tokens)->next;
	}
}

t_bool	is_redirect_token(t_token *tkn)
{
	return (tkn->type == _double_greater || tkn->type == _greater
		|| tkn->type == _io || tkn->type == _lesser);
}

void	del_redirect_list(t_redirect **redirect_p)
{
	t_redirect	*now;
	t_redirect	*tmp;

	if (!redirect_p)
		return ;
	now = *redirect_p;
	while (now)
	{
		tmp = now->next;
		free_tokens(&now->filename);
		free(now);
		now = tmp;
	}
	*redirect_p = 0;
}

void	free_names(t_node **node)
{
	if (!node || !*node)
		return ;
	if ((*node)->type == _node_type_cmd && (*node)->command)
	{
		free_tokens(&(*node)->command->args);
		del_redirect_list(&(*node)->command->redirects);
		free((*node)->command);
	}
	free_names(&(*node)->left);
	free_names(&(*node)->right);
	free(*node);
	*node = 0;
}

t_redirect	*create_redirect(void)
{
	t_redirect	*redirect;

	redirect = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redirect)
		err_ext();
	redirect->fd_io = -1;
	redirect->prev = 0;
	redirect->filename = 0;
	redirect->fd_file = -1;
	redirect->fd_backup = -1;
	redirect->next = 0;
	return (redirect);
}

static void	set_redirect_fd(t_redirect *redirect)
{
	if (redirect->fd_io == -1)
	{
		if (redirect->type == _red_input)
			redirect->fd_io = 0;
		else
			redirect->fd_io = 1;
	}
}

void	add_redirect(t_redirect **redir_lst, t_redirect *new_elem)
{
	t_redirect	*now;

	if (!*redir_lst)
		*redir_lst = new_elem;
	else
	{
		now = *redir_lst;
		while (now->next)
			now = now->next;
		new_elem->next = 0;
		new_elem->prev = now;
		now->next = new_elem;
	}
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

t_bool	has_token_type(t_token **tok, t_char_types tok_type)
{
	if ((*tok)->type == tok_type)
	{
		*tok = (*tok)->next;
		return (1);
	}
	return (0);
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

static t_bool	parse_pipeline(
		t_pre_command		**last_command, t_node **node, t_token **tokens)
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
