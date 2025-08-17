/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 06:26:16 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/13 19:45:11 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "libmem.h"
#include "expander.h"

int	testdir(char *dir)
{
	struct stat	finfo;
	int			r;

	r = stat(dir, &finfo);
	if (r < 0)
		return (-1);
	if (S_ISLNK(finfo.st_mode))
		return (-2);
	if (S_ISDIR(finfo.st_mode) == 0)
		return (-1);
	return (0);
}

char	**arraydir(char *dir, char **array)
{
	int		i;
	int		l;
	char	**result;

	l = ft_strlen(dir);
	if (!l)
		return (array);
	i = 0;
	while (array[i])
		++i;
	result = mem_malloc((i + 1) * sizeof(char *));
	i = -1;
	while (array[++i])
	{
		result[i] = mem_malloc(l + ft_strlen(array[i]) + 3);
		ft_strcpy(result[i], dir);
		if (dir[l - 1] != '/')
			result[i][l] = '/';
		if (array[i][0])
			ft_strcpy(result[i] + l + (dir[l - 1] != '/'), array[i]);
		else
			result[i][l + (dir[l - 1] != '/')] = '\0';
	}
	result[i] = NULL;
	return (result);
}

void	finddir(char **r, int *count, t_list **lastlink)
{
	int		ndirs;
	t_list	*ret;
	t_list	*e;
	t_list	*g;

	ret = NULL;
	e = NULL;
	if (!r || !*r)
		return ;
	ndirs = -1;
	while (r[++ndirs])
	{
		g = mem_malloc(sizeof(t_list));
		if (!e)
			e = g;
		g->next = ret;
		ret = g;
		g->content = r[ndirs];
	}
	if (ndirs)
	{
		e->next = *lastlink;
		*lastlink = ret;
		*count += ndirs;
	}
}
