/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	true_size(size_t given, size_t source, unsigned int start)
{
	size_t	true_src;

	if (source - start < 1)
		return (1);
	else
		true_src = source - start;
	if (given == true_src)
		return (given);
	if (given < true_src)
		return (given);
	else
		return (true_src);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*new_str;
	unsigned int	i;
	size_t			sizes;

	i = start;
	if (s == 0)
		return (0);
	if (ft_strlen(s) < start)
	{
		new_str = malloc(1);
		new_str[0] = '\0';
		return (new_str);
	}
	sizes = true_size(len, ft_strlen(s), start);
	new_str = malloc(sizes + 1);
	if (new_str == 0)
		return (0);
	while (i--)
		s++;
	ft_memcpy(new_str, s, sizes);
	new_str[sizes] = '\0';
	return (new_str);
}
