/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_vector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:22:32 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/13 03:11:05 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include "libmem.h"
#include "expander.h"

static t_list	*make_node(t_list *next, void *content, unsigned int *count)
{
	t_list	*res;

	res = mem_malloc(sizeof(t_list));
	res->next = next;
	res->content = content;
	*count += 1;
	return (res);
}

char	**glob_vector(char *pat, char *dir, int flags)
{
	DIR				*d;
	struct dirent	*dp;
	t_list			*lastlink;
	struct stat		finfo;
	t_list			*e;
	t_list			*dirlist;
	int				ndirs;
	char			*subdir;
	unsigned int	count;
	char			**name_vector;
	unsigned int	i;
	int				pflags;
	t_list			*tmplink;
	t_finddir_args	args;

	lastlink = NULL;
	count = 0;
	name_vector = NULL;
	if ((!pat || !*pat || !glob_pattern(pat)) && testdir(dir) < 0)
		return (NULL);
	if (!pat || !*pat)
		lastlink = make_node(NULL, ft_strdup(""), &count);
	else if (!glob_pattern(pat) && lstat(ft_strjoin(ft_strjoin(dir, "/"),
		pat), &finfo))
		lastlink = make_node(NULL, ft_strdup(pat), &count);
	else if (glob_pattern(pat))
	{
		d = opendir(dir);
		if (!d)
			return (NULL);
		while (1)
		{
			dp = readdir(d);
			if (!dp)
				break ;
			if (*pat != '.' && *(dp->d_name) == '.' && (!dp->d_name[1]
					|| (dp->d_name[1] == '.' && !dp->d_name[2])))
				continue ;
			if (flags & GX_ALLDIRS)
			{
				pflags = 0 + (flags & GX_ALLDIRS) * MP_RMDOT;
				if (flags & GX_NULLDIR)
					pflags |= MP_IGNDOT;
				subdir = glob_makepath(dir, dp->d_name, pflags);
				if (!testdir(subdir))
				{
					args.pat = pat;
					args.sdir = subdir;
					args.flags = (flags & ~GX_ADDCURDIR);
					args.ep = &e;
					args.np = &ndirs;
					dirlist = finddir(args);
					if (ndirs)
					{
						e->next = lastlink;
						lastlink = dirlist;
						count += ndirs;
					}
				}
				lastlink = make_node(lastlink, ft_strdup(subdir), &count);
			}
			if ((!ft_strchr(pat, '.') && (!ft_strcmp(dp->d_name, ".")
					|| !ft_strcmp(dp->d_name, ".."))) || (*(dp->d_name) == '.'
					&& *pat != '.') || flags & GX_ALLDIRS)
				continue ;
			if (!glob_match(pat, dp->d_name))
				lastlink = make_node(lastlink, ft_strdup(dp->d_name), &count);
		}
		closedir(d);
	}
	if ((flags & (GX_ALLDIRS | GX_ADDCURDIR)) == (GX_ALLDIRS | GX_ADDCURDIR)
		&& flags & GX_NULLDIR)
			lastlink = make_node(lastlink, ft_strdup(""), &count);
	else if ((flags & (GX_ALLDIRS | GX_ADDCURDIR)) == (GX_ALLDIRS |
		 GX_ADDCURDIR))
			lastlink = make_node(lastlink, ft_strdup(dir), &count);
	name_vector = mem_malloc((count + 1) * sizeof(char *));
	tmplink = lastlink;
	i = -1;
	while (++i < count)
	{
		name_vector[i] = tmplink->content;
		tmplink = tmplink->next;
	}
	name_vector[count] = NULL;
	return (name_vector);
}
