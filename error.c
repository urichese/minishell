/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 18:04:20 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 17:25:17 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "minishell.h"

void	smart_free(void **thing)
{
	if (*thing != NULL)
		free(*thing);
}

void	err_ext(void)
{
	perror(strerror(errno));
	exit(1);
}
