/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_vector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:22:32 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/11 17:23:19 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include "libmem.h"
#include "expander.h"

char	**glob_vector(char *pat, char *dir, int flags)
{
	DIR				*d;
	struct dirent	*dp;
	t_list			*lastlink;
	struct stat		finfo;
	t_list			*e;
	t_list			*dirlist;
	int				ndirs;
	t_list			*nextlink;
	char			*nextname;
	char			*npat;
	char			*subdir;
	unsigned int	count;
	int				skip;
	int				add_current;
	char			**name_vector;
	unsigned int	i;
	int				pflags;
	t_list			*tmplink;
	t_finddir_args	args;

	lastlink = 0;
	count = 0;
	skip = 0;
	add_current = 0;
	name_vector = NULL;
	if (!pat || !*pat)
	{
		if (testdir(dir) < 0)
			return (NULL);
		nextlink = mem_malloc(sizeof(t_list));
		nextlink->next = NULL;
		lastlink = nextlink;
		nextlink->content = ft_strdup("");
		count = 1;
		skip = 1;
	}
	if (!skip && !glob_pattern(pat))
	{
		if (testdir(dir) < 0)
			return (NULL);
		nextname = mem_malloc(ft_strlen(dir) + ft_strlen(pat) + 2);
		npat = ft_strdup(pat);
		ft_strcpy(nextname, dir);
		nextname[ft_strlen(dir)] = '/';
		ft_strcpy(nextname + ft_strlen(dir) + 1, npat);
		if (lstat(nextname, &finfo))
		{
			nextlink = mem_malloc(sizeof(t_list));
			nextlink->next = NULL;
			lastlink = nextlink;
			nextlink->content = npat;
			count = 1;
		}
		skip = 1;
	}
	if (!skip)
	{
		d = opendir(dir);
		if (!d)
			return (NULL);
		add_current = ((flags & (GX_ALLDIRS | GX_ADDCURDIR))
				== (GX_ALLDIRS | GX_ADDCURDIR));
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
				if (flags & GX_ALLDIRS)
					pflags = MP_RMDOT;
				else
					pflags = 0;
				if (flags & GX_NULLDIR)
					pflags |= MP_IGNDOT;
				subdir = glob_makepath(dir, dp->d_name, pflags);
			}
			if (flags & GX_ALLDIRS)
			{
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
				nextlink = mem_malloc(sizeof(t_list));
				nextname = ft_strdup(subdir);
				nextlink->next = lastlink;
				lastlink = nextlink;
				nextlink->content = nextname;
				count++;
				continue ;
			}
			if (!ft_strchr(pat, '.') && (!ft_strcmp(dp->d_name, ".")
					|| !ft_strcmp(dp->d_name, "..")))
				continue ;
			if (*(dp->d_name) == '.' && *pat != '.')
				continue ;
			if (!glob_match(pat, dp->d_name))
			{
				nextlink = mem_malloc(sizeof(t_list));
				nextname = ft_strdup(dp->d_name);
				nextlink->next = lastlink;
				lastlink = nextlink;
				nextlink->content = nextname;
				count++;
			}
		}
		closedir(d);
	}
	if (add_current)
	{
		nextname = mem_malloc(ft_strlen(dir) + 1);
		nextlink = mem_malloc(sizeof(t_list));
		nextlink->content = nextname;
		nextlink->next = lastlink;
		lastlink = nextlink;
		if (flags & GX_NULLDIR)
			*nextname = '\0';
		else
			ft_memcpy(nextname, dir, ft_strlen(dir) + 1);
		count++;
	}
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
