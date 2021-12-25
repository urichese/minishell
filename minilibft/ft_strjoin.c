/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <mlothair@student.21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:15:09 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 18:23:31 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*new_str;
	unsigned int	i;
	size_t			size;

	if (s1 == 0)
		return (0);
	size = ft_strlen(s1) + ft_strlen(s2);
	new_str = malloc(size + 1);
	if (new_str == 0)
		return (0);
	if (size == 0)
	{
		new_str[0] = 0;
		return (new_str);
	}
	ft_memcpy(new_str, s1, ft_strlen(s1));
	i = ft_strlen(s1);
	while (i < size)
	{
		new_str[i] = *s2;
		s2++;
		i++;
	}
	new_str[size] = '\0';
	return (new_str);
}
