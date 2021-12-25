/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 18:04:20 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:25:04 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_environments	*env_merge(t_environments *left,
									t_environments *right, int (*cmp)())
{
	t_environments	elem;
	t_environments	*next;

	next = &elem;
	while (left != NULL && right != NULL)
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
	if (left == NULL)
		next->next = right;
	else
		next->next = left;
	return (elem.next);
}

static t_environments	*env_mergesort_sub(t_environments *lst, int (*cmp)())
{
	t_environments	*left;
	t_environments	*right;
	t_environments	*right_head;

	if (lst == NULL || lst->next == NULL)
		return (lst);
	left = lst;
	right = lst->next;
	if (right != NULL)
		right = right->next;
	while (right != NULL)
	{
		left = left->next;
		right = right->next;
		if (right != NULL)
			right = right->next;
	}
	right_head = left->next;
	left->next = NULL;
	return (env_merge(env_mergesort_sub(lst, cmp),
			env_mergesort_sub(right_head, cmp), cmp));
}

void	env_mergesort(t_environments **lst, int (*cmp)())
{
	*lst = env_mergesort_sub(*lst, cmp);
}

static void	print_env(t_environments *env)
{
	char	*escaped_value;

	if (env->is_environment == 0)
		return ;
	printf("declare -x %s", env->name);
	if (env->value)
	{
		escaped_value = create_expanded_str(env->value, in_dquoted, 1);
		printf("=\"%s\"", escaped_value);
		free(escaped_value);
	}
	printf("\n");
}
