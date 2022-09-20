/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_sorting.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 18:04:20 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:25:04 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_environments	*env_merge(t_environments *left,
									t_environments *right, int (*cmp)())
{
	t_environments	elem;
	t_environments	*next;

	next = &elem;
	while (left != 0 && right != 0)
	{
		if (cmp(left, right) < 0)
		{
			next->next = left;
			next = next->next;
			left = left->next;
		}
		else
		{
			next->next = right;
			next = next->next;
			right = right->next;
		}
	}
	if (left == 0)
		next->next = right;
	else
		next->next = left;
	return (elem.next);
}

t_environments	*env_mergesort_sub(t_environments *lst, int (*cmp)())
{
	t_environments	*left;
	t_environments	*right;
	t_environments	*right_head;

	if (lst == 0 || lst->next == 0)
		return (lst);
	left = lst;
	right = lst->next;
	if (right != 0)
		right = right->next;
	while (right != 0)
	{
		left = left->next;
		right = right->next;
		if (right != 0)
			right = right->next;
	}
	right_head = left->next;
	left->next = 0;
	return (env_merge(env_mergesort_sub(lst, cmp),
			env_mergesort_sub(right_head, cmp), cmp));
}

int	comparison(t_environments *left, t_environments *right)
{
	return (ft_strcmp(left->name, right->name));
}

void	ft_sorting(t_environments **lst)
{
	*lst = env_mergesort_sub(*lst, comparison);
}
