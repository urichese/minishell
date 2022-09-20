/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	char	*src_c;
	size_t	size_copy;

	size_copy = dstsize - 1;
	src_c = (char *) src;
	if (dstsize == 0)
		return (ft_strlen(src));
	while (size_copy > 0)
	{
		if (*src_c == '\0')
			break ;
		*dst = *src_c;
		dst++;
		src_c++;
		size_copy--;
	}
	if (dstsize >= 0)
		*dst = '\0';
	return (ft_strlen(src));
}
