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

void	set_redirect_fd(t_redirect *redirect)
{
	if (redirect->fd_io == -1)
	{
		if (redirect->type == _red_input || redirect->type == _red_d_input)
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
	else if (tkn->type == _double_lesser)
		redirect->type = _red_d_input;
	else if (tkn->type == _greater)
		redirect->type = _red_output;
	else if (tkn->type == _double_greater)
		redirect->type = _red_d_output;
	else
		return (0);
	set_redirect_fd(redirect);
	return (1);
}
