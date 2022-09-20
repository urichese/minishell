/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlothair <mlothair@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 19:55:07 by mlothair          #+#    #+#             */
/*   Updated: 2021/11/29 19:28:57 by mlothair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	remove_c(char *src, char c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (src[i] == c)
			src[i] = '\0';
		i++;
	}
}

static size_t	count_separated(char *src, size_t len)
{
	size_t	i;
	size_t	res;

	i = 0;
	res = 0;
	while (i < len)
	{
		if (src[i] != '\0')
		{
			i += ft_strlen(&src[i]);
			res++;
		}
		i++;
	}
	return (res);
}

static t_bool	set_separated(char *src, size_t len, char **result)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (src[i] != '\0')
		{
			result[j] = ft_strdup(&src[i]);
			if (!result[j])
			{
				while (j > 0)
					free(result[--j]);
				return (0);
			}
			i += ft_strlen(&src[i]);
			j++;
		}
		i++;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	*src;
	size_t	sep_count;
	char	**res;

	if (!s)
		return (0);
	src = ft_strdup(s);
	if (!src)
		return (0);
	remove_c(src, c, ft_strlen(s));
	sep_count = count_separated(src, ft_strlen(s));
	res = malloc(sizeof(char *) * (sep_count + 1));
	if (!res)
		return (0);
	res[sep_count] = 0;
	if (!set_separated(src, ft_strlen(s), res))
	{
		free(res);
		res = 0;
	}
	free(src);
	return (res);
}
