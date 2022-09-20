/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
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

int	ft_atoi_over(const char *str)
{
	int				i;
	int				sign;
	unsigned long	ov_div;
	unsigned long	result;

	i = 0;
	result = 0;
	ov_div = 9223372036854775807 / 10;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	sign = 1;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ('0' <= str[i] && str[i] <= '9'))
	{
		if (((ov_div < result || (ov_div == result && str[i] > '7'))
				&& sign == 1) || ((ov_div < result
					|| (ov_div == result && str[i] > '8')) && sign == -1))
			return (0);
		result *= 10;
		result += str[i++] - '0';
	}
	return ((int)result * sign);
}

t_bool	ft_atoi_by_ref(const char *str, int *return_value)
{
	int long long	i;
	int				sign;
	unsigned long	result;

	i = 0;
	result = 0;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	sign = 1;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ('0' <= str[i] && str[i] <= '9'))
	{
		if (i > 2147483647 && sign == 1)
			return (-1);
		if (i > 2147483648 && sign == -1)
			return (0);
		result *= 10;
		result += str[i++] - '0';
	}
	*return_value = (int) result * sign;
	return (1);
}
