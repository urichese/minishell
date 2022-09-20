/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

static unsigned long long	ft_len_of_int(int a)
{
	unsigned long long	c;

	c = 0;
	if (a == 0)
		return (1);
	if (a < 0)
		c++;
	while (a)
	{
		c++;
		a /= 10;
	}
	return (c);
}

static void	ft_str_rev(char *s)
{
	unsigned int		i;
	unsigned int		j;
	unsigned int		size;
	char				tmp;

	i = 0;
	size = ft_strlen(s);
	j = size - 1;
	while (i < size / 2)
	{
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
		i++;
		j--;
	}
}

static unsigned int	ft_abs(int a)
{
	return (a * ((a > 0) - (a < 0)));
}

char	*ft_itoa(int n)
{
	unsigned int	nn;
	char			*str_rev;
	int				i;

	i = 0;
	nn = ft_abs(n);
	str_rev = malloc(ft_len_of_int(n) + 1);
	if (str_rev == 0)
		return (0);
	while (nn)
	{
		str_rev[i] = 48 + (nn % 10);
		i++;
		nn /= 10;
	}
	if (n < 0)
		str_rev[i] = '-';
	str_rev[ft_len_of_int(n)] = '\0';
	ft_str_rev(str_rev);
	if (n == 0)
		str_rev[0] = '0';
	str_rev[ft_len_of_int(n)] = '\0';
	return (str_rev);
}
