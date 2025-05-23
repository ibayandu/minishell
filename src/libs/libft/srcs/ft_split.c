/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:30:50 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/04 01:46:20 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_word(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static char	*get_word(char const *s, char c, size_t index)
{
	char	*res;
	size_t	len;
	size_t	i;

	len = 0;
	while (s[index + len] && s[index + len] != c)
		len++;
	res = ft_malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = s[index + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	i;
	size_t	j;

	res = ft_malloc(sizeof(char *) * (count_word(s, c) + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count_word(s, c))
	{
		while (s[j] && s[j] == c)
			j++;
		if (s[j])
		{
			res[i] = get_word(s, c, j);
			if (!(res[i]))
				return (NULL);
			i++;
		}
		while (s[j] && s[j] != c)
			j++;
	}
	res[i] = NULL;
	return (res);
}
