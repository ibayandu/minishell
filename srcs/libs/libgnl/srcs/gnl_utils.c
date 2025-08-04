/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:17:04 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 16:23:07 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "gnl_utils.h"

size_t	gnl_strlen(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*gnl_find_newline(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == (char) '\n')
			return ((char *)&str[i]);
		i++;
	}
	return (NULL);
}

size_t	gnl_strlcpy(char *dst, char *src, size_t dstsize)
{
	size_t	i;
	size_t	srclen;

	srclen = 0;
	while (src[srclen])
		srclen++;
	if (!dstsize)
		return (srclen);
	i = 0;
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize)
		dst[i] = '\0';
	return (srclen);
}

char	*gnl_strjoin(char *s1, char *s2)
{
	size_t	i;
	char	*res;

	res = mem_malloc((gnl_strlen(s1) + gnl_strlen(s2) + 1) * sizeof(char));
	i = 0;
	while (s1 && *s1)
	{
		res[i++] = *s1;
		s1++;
	}
	while (s2 && *s2)
	{
		res[i++] = *s2;
		s2++;
	}
	res[i] = '\0';
	return (res);
}

char	*gnl_strdup(char *str)
{
	char	*newstr;
	size_t	strlen;

	strlen = gnl_strlen(str);
	newstr = mem_calloc(strlen + 1, sizeof(char));
	gnl_strlcpy(newstr, str, strlen);
	return (newstr);
}
