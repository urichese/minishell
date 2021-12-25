/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <mlothair@student.21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:15:09 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 18:23:31 by mlothair         ###   ########.fr       */
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
