/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s1u;
	unsigned char	*s2u;

	s1u = (unsigned char *) s1;
	s2u = (unsigned char *) s2;
	while ((*s1u > 0 || *s2u > 0) && (n > 0))
	{
		if (*s1u != *s2u)
			return ((*s1u - *s2u));
		s1u++;
		s2u++;
		n--;
	}
	return ((unsigned char) 0);
}
