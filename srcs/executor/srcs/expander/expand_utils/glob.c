/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:06:51 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/10 03:32:20 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

t_word_list	*glob_restar(t_word_list *list)
{
	t_word_list	*cur;
	char		*s;
	size_t		i;

	cur = list;
	while (cur)
	{
		if (cur->word && cur->word->word)
		{
			s = cur->word->word;
			i = 0;
			while (s[i])
			{
				if (s[i] == CTLESC)
					s[i] = '*';
				i++;
			}
		}
		cur = cur->next;
	}
	return (list);
}

int	glob_pattern(char *string)
{
	int	i;

	if (!string)
		return (0);
	i = -1;
	while (string[++i])
		if (string[i] == CTLESC)
			return (1);
	return (0);
}

int	glob_match(const char *pattern, const char *string)
{
	if (!*pattern)
		return (*string);
	if (*pattern == CTLESC)
	{
		while (*pattern == CTLESC)
			pattern++;
		if (!*pattern)
			return (0);
		while (*string)
		{
			if (!glob_match(pattern, string))
				return (0);
			string++;
		}
		return (glob_match(pattern, string));
	}
	if (*string && *pattern == *string)
		return (glob_match(pattern + 1, string + 1));
	return (1);
}

char	*glob_makepath(char *path, char *dir, int flags)
{
	int		dirlen;
	char	*ret;
	char	*xpath;
	char	*r;

	xpath = path;
	if (!path || !*path)
		xpath = ft_strdup(".");
	else if ((flags & MP_IGNDOT) && path[0] == '.' && (!path[1]
			|| (path[1] == '/' && !path[2])))
		xpath = ft_strdup("");
	dirlen = ft_strlen(dir) - 2 * ((flags & MP_RMDOT) && dir[0] == '.'
			&& dir[1] == '/');
	dir += 2 * ((flags & MP_RMDOT) && dir[0] == '.' && dir[1] == '/');
	ret = mem_malloc(2 + dirlen + ft_strlen(xpath));
	r = ret;
	while (*xpath)
		*r++ = *xpath++;
	if (r > ret && xpath[-1] != '/')
		*r++ = '/';
	while (*dir)
		*r++ = *dir++;
	*r = '\0';
	return (ret);
}
