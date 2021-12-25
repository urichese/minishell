/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <mlothair@student.21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:15:09 by mlothair          #+#    #+#             */
/*   Updated: 2021/10/05 16:22:34 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ss;
	int		size;

	size = ft_strlen(s);
	ss = (char *) s;
	while (*ss)
		ss++;
	while (size > -1)
	{
		if (*ss == (char) c)
			return (ss);
		ss--;
		size--;
	}
	return (0);
}
