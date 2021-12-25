/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <mlothair@student.21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:15:09 by mlothair          #+#    #+#             */
/*   Updated: 2021/10/05 16:22:34 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_atoi(const char *str)
{
	unsigned long long			i;
	int							p;
	long int					res;

	i = 0;
	p = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
	{
		str++;
	}
	if (*str == 45)
		p = -1;
	if (*str == 45 || *str == 43)
		str++;
	while (*str >= '0' && *str <= '9')
	{
		i = ((*str) - 48) + (i * 10);
		if (i > 2147483647 && p == 1)
			return (-1);
		if (i > 2147483648 && p == -1)
			return (0);
		str++;
	}
	res = p * (int) i;
	return (res);
}

t_bool	ft_atoi_with_limit(const char *str, int *return_value)
{
	int				i;
	int				sign;
	unsigned long	div;
	unsigned long	result;

	i = 0;
	result = 0;
	div = 2147483647 / 10;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	sign = 1;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ('0' <= str[i] && str[i] <= '9'))
	{
		if ((div < result || (div == result && str[i] > '7')) && sign == 1)
			return (0);
		else if ((div < result || (div == result && str[i] > '8'))
			&& sign == -1)
			return (0);
		result *= 10;
		result += str[i++] - '0';
	}
	*return_value = (int)result * sign;
	return (1);
}
