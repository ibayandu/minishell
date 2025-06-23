#include <sys/stat.h>
#include <dirent.h>
#include "makers.h"
#include "libft.h"
#include "utils.h"
#include <glob.h>
#include "expander.h"

char	**glob_vector(char *pat, char *dir, int flags, t_minishell *minishell);

int	unquoted_glob_pattern_p(char *string)
{
	int		i;

	if (!string)
		return (0);
	i = -1;
	while (string[++i])
		if (string[i] == '*')
			return (1);
	return (0);
}

int	glob_testdir(char *dir)
{
	struct stat	finfo;
	int			r;

	r = stat (dir, &finfo);
	if (r < 0)
		return (-1);
	if (S_ISLNK (finfo.st_mode))
		return (-2);
	if (S_ISDIR (finfo.st_mode) == 0)
		return (-1);

	return (0);
}

char	*sh_makepath(const char *path, const char *dir, int flags, t_minishell *minishell)
{
	int			dirlen;
	int			pathlen;
	char		*ret;
	t_variable	*v;
	char		*xpath;
	char		*xdir;
	char		*r;
	char		*s;

	if (!path || !*path)
	{
		if (flags & MP_DOCWD)
		{
			xpath = ft_strdup(getcwd(0,0));
			if (!xpath)
			{
				ret = NULL;
				v = find_variable_internal("PWD", minishell);
				if (v)
					ret = v->value;
				if (ret)
					xpath = ft_strdup(ret);
			}
			if (xpath == 0)
			{
				xpath = ft_strdup(".");
				pathlen = 1;
			}
			else
				pathlen = ft_strlen(xpath);
		}
		else
		{
			xpath = ft_strdup(".");
			pathlen = 1;
		}
	}
	else if ((flags & MP_IGNDOT) && path[0] == '.' && (path[1] == '\0' || (path[1] == '/' && path[2] == '\0')))
	{
		xpath = ft_strdup("");
		pathlen = 0;
	}
	else
	{
		xpath = (char *)path;
		pathlen = ft_strlen(xpath);
	}
	xdir = (char *)dir;
	dirlen = ft_strlen(xdir);
	if ((flags & MP_RMDOT) && dir[0] == '.' && dir[1] == '/')
	{
		xdir += 2;
		dirlen -= 2;
	}
	r = ret = ft_malloc(2 + dirlen + pathlen);
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

t_glob	*finddirs (char *pat, char *sdir, int flags, t_glob **ep, int *np, t_minishell *minishell)
{
	char	**r;
	int		ndirs;
	t_glob	*ret;
	t_glob	*e;
	t_glob	*g;

	e = ret = 0;
	r = glob_vector(pat, sdir, flags, minishell);
	if (!r || !*r)
	{
		if (np)
			*np = 0;
		if (ep)
			*ep = 0;
		return (NULL);
	}
	for (ndirs = 0; r[ndirs] != 0; ndirs++)
	{
		g = ft_malloc(sizeof(t_glob));
		if (!e)
			e = g;
		g->next = ret;
		ret = g;
		g->name = r[ndirs];
	}
	if (ep)
		*ep = e;
	if (np)
		*np = ndirs;
	return ret;
}

int strmatch(const char *pattern, const char *string)
{
	/* If we reach end of pattern, success only if string also finished */
	if (*pattern == '\0')
		return *string != '\0';

	/* Handle '*' wildcard: matches zero or more characters */
	if (*pattern == '*') {
		/* Skip consecutive '*' for efficiency */
		while (*pattern == '*')
			pattern++;
		/* If pattern is now at end, it's a match */
		if (*pattern == '\0')
			return 0;
		/* Try to match the rest of the pattern at each position in string */
		for (; *string != '\0'; string++) {
			if (strmatch(pattern, string) == 0)
				return 0;
		}
		/* Also allow '*' to match empty tail */
		return strmatch(pattern, string);
	}

	/* Literal character: must match exactly */
	if (*string != '\0' && *pattern == *string)
		return strmatch(pattern + 1, string + 1);

	/* No match */
	return 1;
}

char	**glob_vector (char *pat, char *dir, int flags, t_minishell *minishell)
{
	DIR				*d;
	struct dirent	*dp;
	t_glob			*lastlink;
	// t_glob			*e;
	// t_glob			*dirlist;
	// int				ndirs;
	t_glob			*nextlink;
	char			*nextname;
	char			*npat;
	char			*subdir;
	unsigned int	count;
	int				lose;
	int				skip;
	int				isdir;
	int				sdlen;
	int				add_current;
	int				patlen;
	char			**name_vector;
	unsigned int	i;
	int				pflags;
	int				nalloca;
	t_glob			*firstmalloc;
	t_glob			*tmplink;
	int				dirlen;

	lastlink = 0;
	count = 0;
	lose = 0;
	skip = 0;
	add_current = 0;
	firstmalloc = 0;
	nalloca = 0;
	name_vector = NULL;
	/* If PAT is empty, skip the loop, but return one (empty) filename. */
	if (!pat || !*pat)
	{
		if (glob_testdir(dir) < 0)
			return (NULL);
		nextlink = ft_malloc(sizeof(t_glob));
		nextlink->next = NULL;
		nextname = ft_malloc(1);
		lastlink = nextlink;
		nextlink->name = nextname;
		nextname[0] = '\0';
		count = 1;
		skip = 1;
	}
	patlen = (pat && *pat) ? ft_strlen(pat) : 0;
	/* If the filename pattern (PAT) does not contain any globbing characters,
		we can dispense with reading the directory, and just see if there is
		a filename `DIR/PAT'.  If there is, and we can access it, just make the
		vector to return and bail immediately. */
	if (skip == 0 && unquoted_glob_pattern_p(pat) == 0)
	{
		if (glob_testdir(dir) < 0)
			return (NULL);
		dirlen = ft_strlen(dir);
		nextname = ft_malloc(dirlen + patlen + 2);
		npat = ft_malloc(patlen + 1);
		ft_strlcpy(npat, pat, ft_strlen(pat));
		ft_strlcpy(nextname, dir, ft_strlen(dir));
		nextname[dirlen++] = '/';
		ft_strlcpy (nextname + dirlen, npat, ft_strlen(npat));
		if (access(nextname, F_OK) >= 0)
		{
			nextlink = ft_malloc(sizeof(t_glob));
			nextlink->next = NULL;
			lastlink = nextlink;
			nextlink->name = npat;
			count = 1;
		}
		skip = 1;
	}
	if (!skip)
	{
		/* Open the directory, punting immediately if we cannot.  If opendir
		is not robust (i.e., it opens non-directories successfully), test
		that DIR is a directory and punt if it's not. */
		d = opendir(dir);
		if (!d)
			return (NULL);
		/* Compute the flags that will be passed to strmatch().  We don't
		need to do this every time through the loop. */
		add_current = ((flags & (GX_ALLDIRS|GX_ADDCURDIR)) == (GX_ALLDIRS|GX_ADDCURDIR));

		/* Scan the directory, finding all names that match	 For each name that matches, allocate a struct globval
		on the stack and store the name in it.
		Chain those structs together; lastlink is the front of the chain.  */
		while (1)
		{
			dp = readdir(d);
			if (!dp)
				break;
			if (pat[0] != '.' && dp->d_name[0] == '.' && (dp->d_name[1] == 0 || (dp->d_name[1] == '.' && dp->d_name[2] == 0)))
				continue;

			/* If we're only interested in directories, don't bother with files */
			if (flags & (GX_ALLDIRS))
			{
				pflags = (flags & GX_ALLDIRS) ? MP_RMDOT : 0;
				if (flags & GX_NULLDIR)
					pflags |= MP_IGNDOT;
				subdir = sh_makepath(dir, dp->d_name, pflags, minishell);
				isdir = glob_testdir (subdir);
			}
			// if (flags & GX_ALLDIRS)
			// {
			// 	if (isdir == 0)
			// 	{
			// 		dirlist = finddirs(pat, subdir, (flags & ~GX_ADDCURDIR), &e, &ndirs, minishell);
			// 		if (ndirs)
			// 		{
			// 			if (!firstmalloc)
			// 				firstmalloc = e;
			// 			e->next = lastlink;
			// 			lastlink = dirlist;
			// 			count += ndirs;
			// 		}
			// 	}

			// 	nextlink = ft_malloc(sizeof(t_glob));
			// 	if (!firstmalloc)
			// 		firstmalloc = nextlink;
			// 	sdlen = ft_strlen(subdir);
			// 	nextname = ft_malloc(sdlen + 1);
			// 	nextlink->next = lastlink;
			// 	lastlink = nextlink;
			// 	nextlink->name = nextname;
			// 	ft_memcpy(nextname, subdir, sdlen + 1);
			// 	++count;
			// 	continue;
			// }
			if (strmatch (pat, dp->d_name) == 0)
			{
				nextlink = ft_malloc(sizeof(t_glob));
				if (firstmalloc == 0)
					firstmalloc = nextlink;
				nextname = ft_malloc(ft_strlen(dp->d_name) + 1);
				nextlink->next = lastlink;
				lastlink = nextlink;
				nextlink->name = nextname;
				ft_memcpy(nextname, dp->d_name, ft_strlen(dp->d_name) + 1);
				++count;
			}
		}
		closedir (d);
	}

	/* compat: if GX_ADDCURDIR, add the passed directory also.  Add an empty
		directory name as a placeholder if GX_NULLDIR (in which case the passed
		directory name is "."). */
	if (add_current && lose == 0)
	{
		sdlen = ft_strlen(dir);
		nextname = ft_malloc(sdlen + 1);
		nextlink = ft_malloc(sizeof(t_glob));
		nextlink->name = nextname;
		nextlink->next = lastlink;
		lastlink = nextlink;
		if (flags & GX_NULLDIR)
			nextname[0] = '\0';
		else
			ft_memcpy(nextname, dir, sdlen + 1);
		++count;
	}
	name_vector = ft_malloc((count + 1) * sizeof(char *));

	/* Copy the name pointers from the linked list into the vector.  */
	for (tmplink = lastlink, i = 0; i < count; ++i)
	{
		name_vector[i] = tmplink->name;
		tmplink = tmplink->next;
	}
	name_vector[count] = NULL;
	/* If we allocated some of the struct globvals, free them now. */
	if (firstmalloc)
	{
		tmplink = 0;
		while (lastlink)
		{
			tmplink = lastlink;
			if (lastlink == firstmalloc)
				lastlink = firstmalloc = 0;
			else
				lastlink = lastlink->next;
		}
	}
	return (name_vector);
}

char	**glob_dir_to_array(char *dir, char **array, int flags)
{
	unsigned int	i;
	unsigned int	l;
	int				add_slash;
	char			**result;
	char			*new;
	struct stat		sb;

	l = ft_strlen(dir);
	if (l == 0)
	{
		if (flags & GX_MARKDIRS)
			for (i = 0; array[i]; i++)
			{
				if ((stat (array[i], &sb) == 0) && S_ISDIR (sb.st_mode))
				{
					l = ft_strlen (array[i]);
					new = ft_realloc(array[i], l + 2);
					new[l] = '/';
					new[l+1] = '\0';
					array[i] = new;
				}
			}
		return (array);
	}
	add_slash = dir[l - 1] != '/';
	i = 0;
	while (array[i] != NULL)
		++i;
	result = ft_malloc((i + 1) * sizeof (char *));

	for (i = 0; array[i] != NULL; i++)
	{
		/* 3 == 1 for NUL, 1 for slash at end of DIR, 1 for GX_MARKDIRS */
		result[i] = ft_malloc(l + ft_strlen (array[i]) + 3);
		ft_strlcpy(result[i], dir, ft_strlen(dir));
		if (add_slash)
			result[i][l] = '/';
		if (array[i][0])
		{
			ft_strlcpy(result[i] + l + add_slash, array[i], ft_strlen(array[i]));
			if (flags & GX_MARKDIRS)
			{
				if ((stat (result[i], &sb) == 0) && S_ISDIR (sb.st_mode))
				{
					size_t rlen;
					rlen = ft_strlen (result[i]);
					result[i][rlen] = '/';
					result[i][rlen+1] = '\0';
				}
			}
		}
		else
			result[i][l+add_slash] = '\0';
	}
	result[i] = NULL;
	return (result);
}

char	**glob_filename(char *pathname, int flags, t_minishell *minishell)
{
	char			**result;
	char			**new_result;
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

	result = ft_malloc(sizeof(char *));
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
		directory_name = ft_malloc(directory_len + 1);
		ft_memcpy(directory_name, pathname, directory_len);
		directory_name[directory_len] = '\0';
		free_dirname = 1;
		++filename;
	}
	if (directory_len > 0 && unquoted_glob_pattern_p(directory_name) == 1)
	{
		all_starstar = 0;
		last_starstar = 0;
		d = directory_name;
		dflags = flags & ~GX_MARKDIRS;
		/* Collapse a sequence of ** patterns separated by one or more slashes
		to a single ** terminated by a slash or NUL */
		if (d[0] == '*' && d[1] == '*' && (d[2] == '/' || d[2] == '\0'))
		{
			p = d;
			while (d[0] == '*' && d[1] == '*' && (d[2] == '/' || d[2] == '\0'))
			{
				p = d;
				if (d[2])
				{
					d += 3;
					while (*d == '/')
						d++;
					if (*d == 0)
						break;
				}
			}
			if (*d == 0)
				all_starstar = 1;
			d = p;
			dflags |= GX_ALLDIRS|GX_ADDCURDIR;
			directory_len = ft_strlen(d);
		}
		/* if there is a non [star][star]/ component in directory_name, we
		still need to collapse trailing sequences of [star][star]/ into
		a single one and note that the directory name ends with [star][star],
		so we can compensate if filename is [star][star] */
		if (all_starstar == 0)
		{
			prev = dl = directory_len;
			while (dl >= 4 && d[dl - 1] == '/' &&
					d[dl - 2] == '*' &&
					d[dl - 3] == '*' &&
					d[dl - 4] == '/')
				prev = dl, dl -= 3;
			if (dl != directory_len)
				last_starstar = 1;
			directory_len = prev;
		}

		/* If the directory name ends in [star][star]/ but the filename is
		[star][star], just remove the final [star][star] from the directory
		so we don't have to scan everything twice. */
		if (last_starstar && directory_len > 4 && filename[0] == '*' && filename[1] == '*' && filename[2] == 0)
			directory_len -= 3;
		if (d[directory_len - 1] == '/')
			d[directory_len - 1] = '\0';
		directories = glob_filename(d, dflags|GX_RECURSE, minishell);
		if (free_dirname)
			directory_name = NULL;
		else if (directories == NULL)
			return (NULL);
		else if (*directories == NULL)
			return (NULL);

		/* If we have something like [star][star]/[star][star], it's no use to
			glob **, then do it again, and throw half the results away.  */
		if (all_starstar && filename[0] == '*' && filename[1] == '*' && filename[2] == 0)
		{
			directory_name = NULL;
			directory_len = 0;
			goto only_filename;
		}
		/* We have successfully globbed the preceding directory name.
		For each name in DIRECTORIES, call glob_vector on it and
		FILENAME.  Concatenate the results together.  */
		for (i = 0; directories[i] != NULL; ++i)
		{
			shouldbreak = 0;

			/* Scan directory even on a NULL filename.  That way, `*h/'
				returns only directories ending in `h', instead of all
				files ending in `h' with a `/' appended. */
			dname = directories[i];
			dflags = flags & ~(GX_MARKDIRS|GX_ALLDIRS|GX_ADDCURDIR);
			/* last_starstar? */
			if (filename[0] == '*' && filename[1] == '*' && filename[2] == '\0')
				dflags |= GX_ALLDIRS|GX_ADDCURDIR;
			if (dname[0] == '\0' && filename[0])
			{
				dflags |= GX_NULLDIR;
				dname = ".";	/* treat null directory name and non-null filename as current directory */
			}
			/* Special handling for symlinks to directories with globstar on */
			if (all_starstar && (dflags & GX_NULLDIR) == 0)
			{
				/* If we have a directory name that is not null (GX_NULLDIR above)
				and is a symlink to a directory, we return the symlink if
				we're not `descending' into it (filename[0] == 0) and return
				glob_error_return (which causes the code below to skip the
				name) otherwise. I should fold this into a test that does both
				checks instead of calling stat twice. */
				if (glob_testdir (dname) == -2 && glob_testdir (dname) == 0)
				{
					if (filename[0] != 0)
						temp_results = NULL;		/* skip */
					else
					{
						/* Construct array to pass to glob_dir_to_array */
						temp_results = ft_malloc(2 * sizeof (char *));
						temp_results[0] = ft_malloc (1);
						**temp_results = '\0';
						temp_results[1] = NULL;
						dflags |= GX_SYMLINK;	/* mostly for debugging */
					}
				}
				else
					temp_results = glob_vector (filename, dname, dflags, minishell);
			}
			else
				temp_results = glob_vector (filename, dname, dflags, minishell);
			if (temp_results)
			{
				char **array;
				register unsigned int l;

				/* If we're expanding **, we don't need to glue the directory
				name to the results; we've already done it in glob_vector */
				if ((dflags & GX_ALLDIRS) && filename[0] == '*' && filename[1] == '*' && (filename[2] == '\0' || filename[2] == '/'))
				{
				/* When do we remove null elements from temp_results?  And
					how to avoid duplicate elements in the final result? */
				/* If (dflags & GX_NULLDIR) glob_filename potentially left a
					NULL placeholder in the temp results just in case
					glob_vector/glob_dir_to_array did something with it, but
					if it didn't, and we're not supposed to be passing them
					through for some reason ((flags & GX_NULLDIR) == 0) we
					need to remove all the NULL elements from the beginning
					of TEMP_RESULTS. */
				/* If we have a null directory name and ** as the filename,
					we have just searched for everything from the current
					directory on down. Break now (shouldbreak = 1) to avoid
					duplicate entries in the final result. */
				if ((dflags & GX_NULLDIR) && (flags & GX_NULLDIR) == 0 && temp_results && *temp_results && **temp_results == 0)
				{
					for (n = 0; temp_results[n] && *temp_results[n] == 0; n++)
						;
					i = n;
					do
					temp_results[i - n] = temp_results[i];
					while (temp_results[i++] != 0);
					array = temp_results;
					shouldbreak = 1;
				}
					else
					array = temp_results;
				}
				else if (dflags & GX_SYMLINK)
					array = glob_dir_to_array(directories[i], temp_results, flags);
				else
					array = glob_dir_to_array (directories[i], temp_results, flags);
				l = 0;
				while (array[l])
					++l;
				new_result = ft_realloc(result, (result_size + l) * sizeof(char *));
				result = new_result;
				for (l = 0; array[l]; ++l)
				result[result_size++ - 1] = array[l];
				result[result_size - 1] = NULL;
				if (shouldbreak)
					break;
			}
		}
		return (result);
	}

	only_filename:
	/* If there is only a directory name, return it. */
	if (!*filename)
	{
		result = ft_realloc(result, 2 * sizeof(char *));
		result[0] = ft_malloc(directory_len + 1);
		ft_memcpy(result[0], directory_name, directory_len + 1);
		result[1] = NULL;
		return (result);
	}
	else
	{
		/* Just return what glob_vector () returns appended to the
		directory name. */
		/* If flags & GX_ALLDIRS, we're called recursively */
		dflags = flags & ~GX_MARKDIRS;
		if (directory_len == 0)
		dflags |= GX_NULLDIR;
		if (filename[0] == '*' && filename[1] == '*' && filename[2] == '\0')
		{
			dflags |= GX_ALLDIRS|GX_ADDCURDIR;
			if (directory_len == 0 && (flags & GX_ALLDIRS) == 0)
				dflags &= ~GX_ADDCURDIR;
		}
		temp_results = glob_vector(filename, (directory_len == 0 ? "." : directory_name), dflags, minishell);
		if (!temp_results || temp_results == NULL)
			return (temp_results);
		result = glob_dir_to_array((dflags & GX_ALLDIRS) ? "" : directory_name, temp_results, flags);
		return (result);
	}
	return (NULL);
}

char	**shell_glob_filename (const char *pathname, t_minishell *minishell)
{
	char	*temp;
	char	**results;

	temp = (char *)pathname;
	results = glob_filename(temp, 0, minishell);
	if (results)
	{
		if (results && results[0])
			strvec_sort(results);
		else
			results = NULL;
	}
	return (results);
}

t_word_list	*glob_expand_word_list(t_word_list *tlist, t_minishell *minishell)
{
	char		**glob_array;
	int			glob_index;
	int			allow_null_glob_expansion;
	t_word_list	*glob_list;
	t_word_list	*output_list;
	t_word_list	*disposables;
	t_word_list	*next;
	t_word		*tword;

	output_list = NULL;
	disposables = NULL;
	glob_array = NULL;
	allow_null_glob_expansion = 0;
	while (tlist)
	{
		next = tlist->next;
		if (unquoted_glob_pattern_p (tlist->word->word))
		{
			glob_array = shell_glob_filename (tlist->word->word, minishell);
			if (!glob_array)
			{
				glob_array = ft_malloc(sizeof (char *));
				glob_array[0] = NULL;
			}
			glob_list = NULL;
			glob_index = 0;
			while (glob_array[glob_index])
			{
				tword = make_bare_word(glob_array[glob_index]);
				glob_list = make_word_list(tword, glob_list);
				glob_index++;
			}
			if (glob_list)
			{
				output_list = list_append(glob_list, output_list);
				tlist->next = disposables;
				disposables = tlist;
			}
			// else if (fail_glob_expansion)
			// {
			// 	last_command_exit_value = EXECUTION_FAILURE;
			// 	report_error (_("no match: %s"), tlist->word->word);
			// 	exp_jump_to_top_level (DISCARD);
			// }
			else if (!allow_null_glob_expansion)
			{
				tlist->next = output_list;
				output_list = tlist;
			}
			else
			{
				tlist->next = disposables;
				disposables = tlist;
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
	if (output_list)
		output_list = ft_revword(output_list);
	return (output_list);
}
