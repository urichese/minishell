/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:53:11 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	set_command_args(t_cmd *command, t_token **tokens)
{
	while (*tokens && (*tokens)->type == _token)
	{
		add_copied_token(&command->args, *tokens);
		*tokens = (*tokens)->next;
	}
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
