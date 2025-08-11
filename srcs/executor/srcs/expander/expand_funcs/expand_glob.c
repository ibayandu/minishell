/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_glob.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:21:53 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/11 16:25:41 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <dirent.h>
#include "libmem.h"
#include "parser_makers.h"
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
			if (pat[0] != '.' && dp->d_name[0] == '.' && (!dp->d_name[1]
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
			if (dp->d_name[0] == '.' && pat[0] != '.')
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
			nextname[0] = '\0';
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

static char	**handle_empty_filename(char *directory_name, char **result)
{
	result = ft_realloc_vec(result, 2 * sizeof(char *));
	result[0] = ft_strdup(directory_name);
	result[1] = NULL;
	return (result);
}

static char	**handle_only_filename(char *directory_name, int directory_len,
	char *filename, int flags)
{
	char	**result;
	int		dflags;
	char	**temp_results;

	dflags = flags;
	if (!directory_len)
		dflags |= GX_NULLDIR;
	if (!ft_strncmp(filename, DCTLESC, 2) && filename[2] != CTLESC)
	{
		dflags |= GX_ALLDIRS | GX_ADDCURDIR;
		if (!directory_len && !(flags & GX_ALLDIRS))
			dflags &= ~GX_ADDCURDIR;
	}
	if (!directory_len)
		temp_results = glob_vector(filename, ".", dflags);
	else
		temp_results = glob_vector(filename, directory_name, dflags);
	if (!temp_results)
		return (NULL);
	if (dflags & GX_ALLDIRS)
		result = arraydir("", temp_results);
	else
		result = arraydir(directory_name, temp_results);
	return (result);
}

static char	**glob_filename(char *pathname, int flags)
{
	char			**result;
	int				result_size;
	char			*directory_name;
	char			*filename;
	char			*dname;
	int				directory_len;
	int				dflags;
	char			**directories;
	char			*d;
	char			*p;
	unsigned int	i;
	int				all_starstar;
	int				last_starstar;
	int				dl;
	int				prev;
	char			**temp_results;
	int				shouldbreak;
	int				n;
	char			**array;
	unsigned int	l;

	result = mem_malloc(sizeof(char *));
	result_size = 1;
	result[0] = NULL;
	directory_name = NULL;
	filename = ft_strrchr(pathname, '/');
	if (!filename)
	{
		filename = pathname;
		directory_name = "";
		directory_len = 0;
	}
	else
	{
		directory_len = (filename++ - pathname) + 1;
		directory_name = mem_calloc(directory_len + 1, 1);
		ft_memcpy(directory_name, pathname, directory_len);
	}
	if (directory_len > 0 && glob_pattern(directory_name) == 1)
	{
		all_starstar = 0;
		last_starstar = 0;
		d = directory_name;
		dflags = flags;
		if (d[0] == CTLESC && d[1] == CTLESC && (d[2] == '/' || !d[2]))
		{
			p = d;
			while (d[0] == CTLESC && d[1] == CTLESC && (d[2] == '/' || !d[2]))
			{
				p = d;
				if (d[2])
				{
					d += 3;
					while (*d == '/')
						d++;
					if (!*d)
						break ;
				}
			}
			all_starstar = 1 * !*d;
			d = p;
			dflags |= GX_ALLDIRS | GX_ADDCURDIR;
			directory_len = ft_strlen(d);
		}
		if (!all_starstar)
		{
			dl = directory_len;
			prev = dl;
			while (dl >= 4 && d[dl - 1] == '/' && d[dl - 2] == CTLESC
				&& d[dl - 3] == CTLESC && d[dl - 4] == '/')
			{
				prev = dl;
				dl -= 3;
			}
			last_starstar = 1 * dl != directory_len;
			directory_len = prev;
		}
		directory_len -= 3 * last_starstar && directory_len > 4
			&& !ft_strncmp(filename, DCTLESC, 2);
		if (d[directory_len - 1] == '/')
			d[directory_len - 1] = '\0';
		directories = glob_filename(d, dflags);
		if (ft_strrchr(pathname, '/'))
			directory_name = NULL;
		else if (!directories || !*directories)
			return (NULL);
		if (all_starstar && !ft_strncmp(filename, DCTLESC, 2))
		{
			if (!*filename)
				return (handle_empty_filename(NULL, result));
			return (handle_only_filename(NULL, 0, filename, flags));
		}
		i = -1;
		while (directories[++i])
		{
			shouldbreak = 0;
			dname = directories[i];
			dflags = flags & ~(GX_ALLDIRS | GX_ADDCURDIR);
			if (filename[0] == CTLESC && filename[1] == CTLESC
				&& !filename[2])
				dflags |= GX_ALLDIRS | GX_ADDCURDIR;
			if (!dname[0] && filename[0])
			{
				dflags |= GX_NULLDIR;
				dname = ".";
			}
			if (all_starstar && !(dflags & GX_NULLDIR))
			{
				if (testdir(dname) == -2 && !testdir(dname))
				{
					if (filename[0] != 0)
						temp_results = NULL;
					else
					{
						temp_results = mem_malloc(2 * sizeof (char *));
						temp_results[0] = ft_strdup("");
						temp_results[1] = NULL;
					}
				}
				else
					temp_results = glob_vector(filename, dname, dflags);
			}
			else
				temp_results = glob_vector(filename, dname, dflags);
			if (temp_results)
			{
				if ((dflags & GX_ALLDIRS) && filename[0] == CTLESC
					&& filename[1] == CTLESC && (!filename[2]
						|| filename[2] == '/'))
				{
					if ((dflags & GX_NULLDIR) && !(flags & GX_NULLDIR)
						&& temp_results && *temp_results
						&& !**temp_results)
					{
						n = 0;
						while (temp_results[n] && *temp_results[n])
							n++;
						i = n;
						temp_results[i - n] = temp_results[i];
						while (temp_results[i++])
							temp_results[i - n] = temp_results[i];
						array = temp_results;
						shouldbreak = 1;
					}
					else
						array = temp_results;
				}
				else
					array = arraydir(directories[i], temp_results);
				l = 0;
				while (array[l])
					++l;
				result = ft_realloc_vec(result,
						(result_size + l) * sizeof(char *));
				l = -1;
				while (array[++l])
					result[result_size++ - 1] = array[l];
				result[result_size - 1] = NULL;
				if (shouldbreak)
					break ;
			}
		}
		return (result);
	}
	if (!*filename)
		return (handle_empty_filename(directory_name, result));
	return (handle_only_filename(directory_name, directory_len,
			filename, flags));
}

static t_word_list	*glob_append(char *word)
{
	char		**glob_array;
	int			glob_index;
	t_word_list	*res;

	glob_array = strvec_sort(glob_filename(word, 0), 1);
	if (!glob_array)
		glob_array = mem_calloc(sizeof(char *), 1);
	res = NULL;
	glob_index = -1;
	while (glob_array[++glob_index])
		res = make_word_list(make_bare_word(glob_array[glob_index]), res);
	return (res);
}

t_word_list	*glob_list(t_word_list *tlist)
{
	t_word_list	*glob_list;
	t_word_list	*output_list;
	t_word_list	*next;

	output_list = NULL;
	while (tlist)
	{
		next = tlist->next;
		if (glob_pattern(tlist->word->word))
			glob_list = glob_append(tlist->word->word);
		if (glob_pattern(tlist->word->word) && glob_list)
			output_list = list_append(glob_list, output_list);
		else
		{
			tlist->next = output_list;
			output_list = tlist;
		}
		tlist = next;
	}
	return (glob_restar(output_list));
}
