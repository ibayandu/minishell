#include <sys/stat.h>
#include <dirent.h>
#include "makers.h"
#include "libft.h"
#include "expander.h"

int	unquoted_glob_pattern_p(char *string)
{
	int		c;

	while (c = *string++)
		if (c == '*')
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
	int		dirlen;
	int		pathlen;
	char	*ret;
	char	*xpath;
	char	*xdir;
	char	*r;
	char	*s;

	if (!path || !*path)
	{
		if (flags & MP_DOCWD)
		{
			xpath = ft_strdup(getcwd(0,0));
			if (!xpath)
			{
				ret = find_variable_internal("PWD", minishell);
				if (ret)
					xpath = ft_strdup(ret);
			}
			if (xpath == 0)
			{
				xpath = ft_strdup(".");
				pathlen = 1;
			}
			else
				pathlen = strlen (xpath);
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
		xpath = path;
		pathlen = ft_strlen(xpath);
	}
	xdir = dir;
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
	while (*r++ = *s++)
		;
	return (ret);
}

t_glob	*finddirs (char *pat, char *sdir, int flags, t_glob **ep, int *np, t_minishell *minishell)
{
	char	**r;
	char	*n;
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
		if (!g)
		{
			while (ret)
			{
				g = ret->next;
				ret = g;
			}
			if (np)
				*np = 0;
			if (ep)
				*ep = 0;
			return (&finddirs_error_return);
		}
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

char	**glob_vector (char *pat, char *dir, int flags, t_minishell *minishell)
{
	DIR				*d;
	struct dirent	*dp;
	t_glob			*lastlink;
	t_glob			*e;
	t_glob			*dirlist;
	t_glob			*nextlink;
	char			*nextname;
	char			*npat;
	char			*subdir;
	unsigned int	count;
	int				lose;
	int				skip;
	int				ndirs;
	int				isdir;
	int				sdlen;
	int				add_current;
	int				patlen;
	char			**name_vector;
	unsigned int	i;
	int				mflags;
	int				pflags;
	int				hasglob;
	int				nalloca;
	t_glob			*firstmalloc;
	t_glob			*tmplink;
	char			*convfn;
	int				dirlen;
	struct stat		finfo;

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
			return (&glob_error_return);
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
		or contains a pattern with only backslash escapes (hasglob == 2),
		we can dispense with reading the directory, and just see if there is
		a filename `DIR/PAT'.  If there is, and we can access it, just make the
		vector to return and bail immediately. */
	hasglob = 0;
	if (skip == 0 && ((hasglob = internal_glob_pattern_p (pat)) == 0 || hasglob == 2))
	{
		if (glob_testdir(dir) < 0)
			return (&glob_error_return);
		dirlen = ft_strlen(dir);
		nextname = ft_malloc(dirlen + patlen + 2);
		npat = ft_malloc(patlen + 1);
		ft_strcpy(npat, pat);
		dequote_pathname (npat);
		ft_strcpy(nextname, dir);
		nextname[dirlen++] = '/';
		ft_strcpy (nextname + dirlen, npat);
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
			return (&glob_error_return);
		/* Compute the flags that will be passed to strmatch().  We don't
		need to do this every time through the loop. */
		mflags = FNM_DOTDOT | FNM_PATHNAME;
		add_current = ((flags & (GX_ALLDIRS|GX_ADDCURDIR)) == (GX_ALLDIRS|GX_ADDCURDIR));

		/* Scan the directory, finding all names that match	 For each name that matches, allocate a struct globval
		on the stack and store the name in it.
		Chain those structs together; lastlink is the front of the chain.  */
		while (1)
		{
			dp = readdir(d);
			if (!dp)
				break;
			if (skipname(pat, dp->d_name, flags))
				continue;

			/* If we're only interested in directories, don't bother with files */
			if (flags & (GX_MATCHDIRS|GX_ALLDIRS))
			{
				pflags = (flags & GX_ALLDIRS) ? MP_RMDOT : 0;
				if (flags & GX_NULLDIR)
					pflags |= MP_IGNDOT;
				subdir = sh_makepath(dir, dp->d_name, pflags, minishell);
				isdir = glob_testdir (subdir);
				if (isdir < 0 && (flags & GX_MATCHDIRS))
					continue;
			}
			if (flags & GX_ALLDIRS)
			{
				if (isdir == 0)
				{
					dirlist = finddirs(pat, subdir, (flags & ~GX_ADDCURDIR), &e, &ndirs, minishell);
					if (dirlist == &finddirs_error_return)
					{
						lose = 1;
						break;
					}
					if (ndirs)
					{
						if (!firstmalloc)
							firstmalloc = e;
						e->next = lastlink;
						lastlink = dirlist;
						count += ndirs;
					}
				}

				nextlink = ft_malloc(sizeof(t_glob));
				if (!firstmalloc)
					firstmalloc = nextlink;
				sdlen = ft_strlen(subdir);
				nextname = ft_malloc(sdlen + 1);
				nextlink->next = lastlink;
				lastlink = nextlink;
				nextlink->name = nextname;
				ft_memcpy(nextname, subdir, sdlen + 1);
				++count;
				continue;
			}
			if (strmatch (pat, dp->d_name, mflags) != FNM_NOMATCH)
			{
				if (nalloca < ALLOCA_MAX)
				{
				nextlink = (struct globval *) alloca (sizeof (struct globval));
				nalloca += sizeof (struct globval);
				}
				else
				{
				nextlink = (struct globval *) malloc (sizeof (struct globval));
				if (firstmalloc == 0)
					firstmalloc = nextlink;
				}

				nextname = (char *) malloc (D_NAMLEN (dp) + 1);
				if (nextlink == 0 || nextname == 0)
				{
				/* We free NEXTLINK here, since it won't be added to the
					LASTLINK chain. If we used malloc, and it returned non-
					NULL, firstmalloc will be set to something valid. If it's
					NEXTLINK, reset it before we free NEXTLINK to avoid
					duplicate frees. If not, it will be taken care of by the
					loop below with TMPLINK. */
				if (firstmalloc)
					{
					if (firstmalloc == nextlink)
					firstmalloc = 0;
					FREE (nextlink);
					}
				FREE (nextname);
				lose = 1;
				break;
				}
				nextlink->next = lastlink;
				lastlink = nextlink;
				nextlink->name = nextname;
				bcopy (dp->d_name, nextname, D_NAMLEN (dp) + 1);
				++count;
			}
		}

		(void) closedir (d);
		}

	/* compat: if GX_ADDCURDIR, add the passed directory also.  Add an empty
		directory name as a placeholder if GX_NULLDIR (in which case the passed
		directory name is "."). */
	if (add_current && lose == 0)
		{
		sdlen = strlen (dir);
		nextname = (char *)malloc (sdlen + 1);
		nextlink = (struct globval *) malloc (sizeof (struct globval));
		if (nextlink == 0 || nextname == 0)
		{
		FREE (nextlink);
		FREE (nextname);
		lose = 1;
		}
		else
		{
		nextlink->name = nextname;
		nextlink->next = lastlink;
		lastlink = nextlink;
		if (flags & GX_NULLDIR)
			nextname[0] = '\0';
		else
			bcopy (dir, nextname, sdlen + 1);
		++count;
		}
		}

	if (lose == 0)
		{
		name_vector = (char **) malloc ((count + 1) * sizeof (char *));
		lose |= name_vector == NULL;
		}

	/* Have we run out of memory or been interrupted? */
	if (lose)
		{
		tmplink = 0;

		/* Here free the strings we have got.  */
		while (lastlink)
		{
		/* Since we build the list in reverse order, the first N entries
			will be allocated with malloc, if firstmalloc is set, from
			lastlink to firstmalloc. */
		if (firstmalloc)
			{
			if (lastlink == firstmalloc)
			firstmalloc = 0;
			tmplink = lastlink;
			}
		else
			tmplink = 0;
		free (lastlink->name);
		lastlink = lastlink->next;
		FREE (tmplink);
		}

		/* Don't call QUIT; here; let higher layers deal with it. */

		return ((char **)NULL);
		}

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
		free (tmplink);
		}
	}
	return (name_vector);
}

char	**glob_filename(char *pathname, int flags)
{
	char			**result;
	char			**new_result;
	int				result_size;
	char			*directory_name;
	char			*filename;
	char			*dname;
	char			*fn;
	int				directory_len;
	int				dflags;
	int				hasglob;
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
	int				dlen;
	int				i;
	int				n;
	char			**temp_results;

	result = ft_malloc(sizeof(char *));
	result_size = 1;
	result[0] = '\0';
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
		ft_memcopy(directory_name, pathname, directory_len);
		directory_name[directory_len] = '\0';
		free_dirname = 1;
		++filename;
	}
	hasglob = 0;
	if (directory_len > 0 && (hasglob = unquoted_glob_pattern_p(directory_name)) == 1)
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
		directories = glob_filename(d, dflags|GX_RECURSE);
		if (free_dirname)
			directory_name = NULL;
		else if (directories == &glob_error_return)
			return (&glob_error_return);
		else if (*directories == NULL)
			return (&glob_error_return);

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
						temp_results = &glob_error_return;		/* skip */
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
					temp_results = glob_vector (filename, dname, dflags);
			}
			else
				temp_results = glob_vector (filename, dname, dflags);
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
		/* If we have a directory name with quoted characters, and we are
		being called recursively to glob the directory portion of a pathname,
		we need to dequote the directory name before returning it so the
		caller can read the directory */
		if (directory_len > 0 && hasglob == 2 && (flags & GX_RECURSE) != 0)
		{
			dequote_pathname (directory_name);
			directory_len = ft_strlen(directory_name);
		}
		/* We could check whether or not the dequoted directory_name is a
		directory and return it here, returning the original directory_name
		if not, but we don't do that. We do return the dequoted directory
		name if we're not being called recursively and the dequoted name
		corresponds to an actual directory. For better backwards compatibility,
		we can return &glob_error_return unconditionally in this case. */
		if (directory_len > 0 && hasglob == 2 && (flags & GX_RECURSE) == 0)
		{
			dequote_pathname (directory_name);
			if (glob_testdir(directory_name) < 0)
				return (&glob_error_return);
		}
		result[0] = ft_malloc(directory_len + 1);
		ft_memcpy(result[0], directory_name, directory_len + 1);
		result[1] = NULL;
		return (result);
	}
	else
	{
		/* There are no unquoted globbing characters in DIRECTORY_NAME.
		Dequote it before we try to open the directory since there may
		be quoted globbing characters which should be treated verbatim. */
		if (directory_len > 0)
			dequote_pathname (directory_name);

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
		temp_results = glob_vector(filename, (directory_len == 0 ? "." : directory_name), dflags);
		if (!temp_results || temp_results == &glob_error_return)
		{
			QUIT;
			return (temp_results);
		}
		result = glob_dir_to_array((dflags & GX_ALLDIRS) ? "" : directory_name, temp_results, flags);
		return (result);
	}
	QUIT;
	return (NULL);
}

char	**shell_glob_filename (const char *pathname, int qflags)
{
	char	*temp;
	char	**results;
	int		gflags;
	int		quoted_pattern;

	noglob_dot_filenames = 0;
	glob_dot_filenames == 0;
	temp = pathname;
	gflags = GX_GLOBSTAR;
	results = glob_filename(temp, gflags);
	if (results && ((GLOB_FAILED (results)) == 0))
	{
		if (should_ignore_glob_matches ())
			ignore_glob_matches (results);
		if (results && results[0])
			strvec_sort (results, 1);
		else
			results = &glob_error_return;
	}
	return (results);
}

int	unquoted_glob_pattern_p(char *string)
{
	int		c;

	while (c = *string++)
		if (c == '*')
			return (1);
	return (0);
}

t_word_list	*glob_expand_word_list(t_word_list *tlist)
{
	char		**glob_array;
	char		*temp_string;
	int			glob_index;
	t_word_list	*glob_list;
	t_word_list	*output_list;
	t_word_list	*disposables;
	t_word_list	*next;
	t_word		*tword;

	output_list = NULL;
	disposables = NULL;
	glob_array = NULL;
	while (tlist)
	{
		next = tlist->next;
		if (unquoted_glob_pattern_p (tlist->word->word))
		{
			glob_array = shell_glob_filename (tlist->word->word, QGLOB_CTLESC);
			if (glob_array == 0 || GLOB_FAILED(glob_array))
			{
				glob_array = ft_malloc(sizeof (char *));
				glob_array[0] = NULL;
			}
			if (glob_array[0] == NULL)
			{
				temp_string = dequote_string (tlist->word->word);
				tlist->word->word = temp_string;
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
				PREPEND_LIST (tlist, disposables);
			}
			else if (fail_glob_expansion)
			{
				last_command_exit_value = EXECUTION_FAILURE;
				report_error (_("no match: %s"), tlist->word->word);
				exp_jump_to_top_level (DISCARD);
			}
			else if (!allow_null_glob_expansion)
				PREPEND_LIST (tlist, output_list);
			else
				PREPEND_LIST (tlist, disposables);
		}
		else
		{
			temp_string = dequote_string (tlist->word->word);
			tlist->word->word = temp_string;
			PREPEND_LIST (tlist, output_list);
		}
		strvec_dispose (glob_array);
		glob_array = NULL;
		tlist = next;
	}
	if (output_list)
		output_list = ft_revword (output_list);
	return (output_list);
}
