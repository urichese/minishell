/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <mlothair@student.21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:15:09 by mlothair          #+#    #+#             */
/*   Updated: 2021/12/22 18:23:31 by mlothair         ###   ########.fr       */
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
