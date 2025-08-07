#include <sys/stat.h>
#include <dirent.h>
#include "libmem.h"
#include "parser_makers.h"
#include "expander.h"

static t_word_list *restar_list(t_word_list *list)
{
    t_word_list *cur = list;

    while (cur)
    {
        if (cur->word && cur->word->word)
        {
            char *s = cur->word->word;
            for (size_t i = 0; s[i] != '\0'; i++)
            {
                if (s[i] == CTLESC)
                    s[i] = '*';
            }
        }
        cur = cur->next;
    }

    return list;
}

static char	**ft_realloc_vec(char **old, size_t new_items)
{
	size_t	old_count;
	char	**newv;
	size_t	to_copy;
	size_t	i;

	old_count = 0;
	if (old)
		while (old[old_count])
			old_count++;
	newv = mem_malloc((new_items + 1) * sizeof *newv);
	to_copy = old_count < new_items ? old_count : new_items;
	i = -1;
	while (++i < to_copy)
		newv[i] = old[i];
	i = to_copy - 1;
	while (++i < new_items)
		newv[i] = NULL;
	newv[new_items] = NULL;
	return (newv);
}

static char	*sh_makepath(char *path, char *dir, int flags)
{
	int			dirlen;
	int			pathlen;
	char		*ret;
	char		*xpath;
	char		*xdir;
	char		*r;
	char		*s;

	if (!path || !*path)
		xpath = ft_strdup(".");
	else if ((flags & MP_IGNDOT) && path[0] == '.' && (path[1] == '\0' || (path[1] == '/' && path[2] == '\0')))
		xpath = ft_strdup("");
	else
		xpath = path;
	pathlen = ft_strlen(xpath);
	xdir = dir;
	dirlen = ft_strlen(xdir);
	if ((flags & MP_RMDOT) && dir[0] == '.' && dir[1] == '/')
	{
		xdir += 2;
		dirlen -= 2;
	}
	ret = mem_malloc(2 + dirlen + pathlen);
	r = ret;
	s = xpath;
	while (*s)
		*r++ = *s++;
	if (s > xpath && s[-1] != '/')
		*r++ = '/';
	s = xdir;
	while ((*r++ = *s++))
		;
	return (ret);
}

static int	glob_pattern(char *string)
{
	int		i;

	if (!string)
		return (0);
	i = -1;
	while (string[++i])
		if (string[i] == CTLESC)
			return (1);
	return (0);
}

static int	glob_match(const char *pattern, const char *string)
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
		add_current = ((flags & (GX_ALLDIRS|GX_ADDCURDIR)) == (GX_ALLDIRS|GX_ADDCURDIR));
		while (1)
		{
			dp = readdir(d);
			if (!dp)
				break ;
			if (pat[0] != '.' && dp->d_name[0] == '.' && (dp->d_name[1] == 0 || (dp->d_name[1] == '.' && dp->d_name[2] == 0)))
				continue ;
			if (flags & GX_ALLDIRS)
			{
				if (flags & GX_ALLDIRS)
					pflags = MP_RMDOT;
				else
					pflags = 0;
				if (flags & GX_NULLDIR)
					pflags |= MP_IGNDOT;
				subdir = sh_makepath(dir, dp->d_name, pflags);
			}
			if (flags & GX_ALLDIRS)
			{
				if (testdir(subdir) == 0)
				{
					t_finddir_args args = {
						.pat = pat,
						.sdir = subdir,
						.flags = (flags & ~GX_ADDCURDIR),
						.ep = &e,
						.np = &ndirs
					};
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
			if (!ft_strchr(pat, '.') && (ft_strcmp(dp->d_name, ".") == 0 || ft_strcmp(dp->d_name, "..") == 0))
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
	int				free_dirname;
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
		free_dirname = 0;
	}
	else
	{
		directory_len = (filename - pathname) + 1;
		directory_name = mem_malloc(directory_len + 1);
		ft_memcpy(directory_name, pathname, directory_len);
		directory_name[directory_len] = '\0';
		free_dirname = 1;
		++filename;
	}
	if (directory_len > 0 && glob_pattern(directory_name) == 1)
	{
		all_starstar = 0;
		last_starstar = 0;
		d = directory_name;
		dflags = flags;
		if (d[0] == CTLESC && d[1] == CTLESC && (d[2] == '/' || d[2] == '\0'))
		{
			p = d;
			while (d[0] == CTLESC && d[1] == CTLESC && (d[2] == '/' || d[2] == '\0'))
			{
				p = d;
				if (d[2])
				{
					d += 3;
					while (*d == '/')
						d++;
					if (!*d)
						break;
				}
			}
			if (!*d)
				all_starstar = 1;
			d = p;
			dflags |= GX_ALLDIRS|GX_ADDCURDIR;
			directory_len = ft_strlen(d);
		}
		if (!all_starstar)
		{
			dl = directory_len;
			prev = dl;
			while (dl >= 4 && d[dl - 1] == '/' && d[dl - 2] == CTLESC && d[dl - 3] == CTLESC && d[dl - 4] == '/')
			{
				prev = dl;
				dl -= 3;
			}
			if (dl != directory_len)
				last_starstar = 1;
			directory_len = prev;
		}
		if (last_starstar && directory_len > 4 && ft_strncmp(filename, "\001\001", 2) == 0)
			directory_len -= 3;
		if (d[directory_len - 1] == '/')
			d[directory_len - 1] = '\0';
		directories = glob_filename(d, dflags);
		if (free_dirname)
			directory_name = NULL;
		else if (directories == NULL)
			return (NULL);
		else if (*directories == NULL)
			return (NULL);
		if (all_starstar && ft_strncmp(filename, "\001\001", 2) == 0)
		{
			directory_name = NULL;
			directory_len = 0;
			goto only_filename;
		}
		i = -1;
		while (directories[++i])
		{
			shouldbreak = 0;
			dname = directories[i];
			dflags = flags & ~(GX_ALLDIRS|GX_ADDCURDIR);
			if (filename[0] == CTLESC && filename[1] == CTLESC && filename[2] == '\0')
				dflags |= GX_ALLDIRS|GX_ADDCURDIR;
			if (dname[0] == '\0' && filename[0])
			{
				dflags |= GX_NULLDIR;
				dname = ".";
			}
			if (all_starstar && (dflags & GX_NULLDIR) == 0)
			{
				if (testdir(dname) == -2 && testdir(dname) == 0)
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
				if ((dflags & GX_ALLDIRS) && filename[0] == CTLESC && filename[1] == CTLESC && (filename[2] == '\0' || filename[2] == '/'))
				{
					if ((dflags & GX_NULLDIR) && (flags & GX_NULLDIR) == 0 && temp_results && *temp_results && **temp_results == 0)
					{
						n = 0;
						while (temp_results[n] && *temp_results[n])
							n++;
						i = n;
						do
							temp_results[i - n] = temp_results[i];
						while (temp_results[i++]);
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
				result = ft_realloc_vec(result, (result_size + l) * sizeof(char *));
				l = -1;
				while (array[++l])
					result[result_size++ - 1] = array[l];
				result[result_size - 1] = NULL;
				if (shouldbreak)
					break;
			}
		}
		return (result);
	}
	only_filename:
	if (!*filename)
	{
		result = ft_realloc_vec(result, 2 * sizeof(char *));
		result[0] = ft_strdup(directory_name);
		result[1] = NULL;
		return (result);
	}
	else
	{
		dflags = flags;
		if (!directory_len)
			dflags |= GX_NULLDIR;
		if (!ft_strncmp(filename, "\001\001", 2) && filename[2] != CTLESC)
		{
			dflags |= GX_ALLDIRS|GX_ADDCURDIR;
			if (!directory_len && !(flags & GX_ALLDIRS))
				dflags &= ~GX_ADDCURDIR;
		}
		temp_results = glob_vector(filename, (!directory_len ? "." : directory_name), dflags);
		if (!temp_results)
			return (NULL);
		result = arraydir((dflags & GX_ALLDIRS) ? "" : directory_name, temp_results);
		return (result);
	}
	return (NULL);
}

t_word_list	*glob_list(t_word_list *tlist)
{
	char		**glob_array;
	int			glob_index;
	t_word_list	*glob_list;
	t_word_list	*output_list;
	t_word_list	*next;

	output_list = NULL;
	glob_array = NULL;
	while (tlist)
	{
		next = tlist->next;
		if (glob_pattern(tlist->word->word))
		{
			glob_array = glob_filename(tlist->word->word, 0);
			if (glob_array && glob_array[0])
				glob_array = strvec_sort(glob_array, 1);
			if (!glob_array)
			{
				glob_array = mem_malloc(sizeof(char *));
				glob_array[0] = NULL;
			}
			glob_list = NULL;
			glob_index = -1;
			while (glob_array[++glob_index])
				glob_list = make_word_list(make_bare_word(glob_array[glob_index]), glob_list);
			if (glob_list)
				output_list = list_append(glob_list, output_list);
			else
			{
				tlist->next = output_list;
				output_list = tlist;
			}
		}
		else
		{
			tlist->next = output_list;
			output_list = tlist;
		}
		glob_array = NULL;
		tlist = next;
	}
	return (restar_list(output_list));
}
