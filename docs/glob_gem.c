/*
** Used to pass the result of splitting a pathname into a directory
** and a filename component.
*/
typedef struct s_path_parts
{
	char	*directory_name;
	int		directory_len;
	int		free_dirname;
}			t_path_parts;

/*
** Used to manage the state during the recursive processing of directories.
** It holds the results array and other necessary context.
*/
typedef struct s_glob_state
{
	char	**result;
	int		result_size;
	char	*filename;
	int		flags;
}			t_glob_state;

/*
** Splits a full path into its directory and filename parts.
** Allocates memory for the directory name if necessary.
*/
static void	split_path(char *pathname, t_path_parts *parts, char **filename)
{
	int	len;

	*filename = ft_strrchr(pathname, '/');
	if (!(*filename))
	{
		*filename = pathname;
		parts->directory_name = "";
		parts->directory_len = 0;
		parts->free_dirname = 0;
	}
	else
	{
		len = (*filename - pathname) + 1;
		parts->directory_name = mem_malloc(len + 1);
		ft_memcpy(parts->directory_name, pathname, len);
		parts->directory_name[len] = '\0';
		parts->directory_len = len;
		parts->free_dirname = 1;
		(*filename)++;
	}
}

static void	append_results(t_glob_state *state, char **new_items)
{
	unsigned int	l;
	unsigned int	i;

	if (!new_items)
		return ;
	l = 0;
	while (new_items[l])
		l++;
	state->result = ft_realloc_vec(state->result,
			(state->result_size + l) * sizeof(char *));
	i = 0;
	while (new_items[i])
	{
		state->result[state->result_size - 1] = new_items[i];
		state->result_size++;
		i++;
	}
	state->result[state->result_size - 1] = NULL;
}

static void	process_directory_entry(t_glob_state *state, char *dname)
{
	char	**temp_results;
	char	**array;
	int		dflags;

	dflags = state->flags & ~(GX_ALLDIRS | GX_ADDCURDIR);
	if (state->filename[0] == CTLESC && state->filename[1] == CTLESC
		&& state->filename[2] == '\0')
		dflags |= GX_ALLDIRS | GX_ADDCURDIR;
	if (dname[0] == '\0' && state->filename[0])
	{
		dflags |= GX_NULLDIR;
		dname = ".";
	}
	temp_results = glob_vector(state->filename, dname, dflags);
	if (temp_results)
	{
		array = arraydir(dname, temp_results);
		append_results(state, array);
	}
}

static char	**handle_recursive_path(t_path_parts *p, char *filename, int flags)
{
	t_glob_state	state;
	char			**directories;
	int				dflags;
	unsigned int	i;

	state.result = mem_malloc(sizeof(char *));
	state.result[0] = NULL;
	state.result_size = 1;
	state.filename = filename;
	state.flags = flags;
	dflags = flags;
	if (p->directory_name[0] == CTLESC && p->directory_name[1] == CTLESC)
		dflags |= GX_ALLDIRS | GX_ADDCURDIR;
	directories = glob_filename(p->directory_name, dflags);
	if (!directories)
	{
		mem_free(state.result);
		return (NULL);
	}
	i = 0;
	while (directories[i])
	{
		process_directory_entry(&state, directories[i]);
		i++;
	}
	return (state.result);
}

static char	**handle_simple_path(t_path_parts *p, char *filename, int flags)
{
	char	**temp_results;
	char	**result;
	int		dflags;

	if (!*filename)
	{
		result = mem_malloc(2 * sizeof(char *));
		result[0] = ft_strdup(p->directory_name);
		result[1] = NULL;
		return (result);
	}
	dflags = flags;
	if (!p->directory_len)
		dflags |= GX_NULLDIR;
	if (!ft_strncmp(filename, "\001\001", 2) && filename[2] != CTLESC)
	{
		dflags |= GX_ALLDIRS | GX_ADDCURDIR;
		if (!p->directory_len && !(flags & GX_ALLDIRS))
			dflags &= ~GX_ADDCURDIR;
	}
	temp_results = glob_vector(filename,
			(!p->directory_len ? "." : p->directory_name), dflags);
	if (!temp_results)
		return (NULL);
	result = arraydir((dflags & GX_ALLDIRS) ? "" : p->directory_name,
			temp_results);
	return (result);
}

static char	**glob_filename(char *pathname, int flags)
{
	t_path_parts	parts;
	char			*filename;
	char			**result;

	split_path(pathname, &parts, &filename);
	if (parts.directory_len > 0 && glob_pattern(parts.directory_name) == 1)
		result = handle_recursive_path(&parts, filename, flags);
	else
		result = handle_simple_path(&parts, filename, flags);
	if (parts.free_dirname)
		mem_free(parts.directory_name);
	return (result);
}

/*
** Holds the state for the glob_vector operation, including the
** list of matches, count, and input parameters. This avoids
** violating the 4-parameter limit in helper functions.
*/
typedef struct s_glob_state
{
	t_list			*list_head;
	unsigned int	count;
	int				flags;
	char			*pat;
	char			*dir;
}					t_glob_state;

/*
** Initializes the state structure for the globbing operation.
*/
static void	init_glob_state(t_glob_state *state, char *p, char *d, int f)
{
	state->list_head = NULL;
	state->count = 0;
	state->pat = p;
	state->dir = d;
	state->flags = f;
}

/*
** Creates a new list node with the given name, adds it to the
** front of the state's list, and increments the match count.
*/
static void	add_match_to_list(t_glob_state *state, char *name)
{
	t_list	*new_link;

	new_link = mem_malloc(sizeof(t_list));
	new_link->content = name;
	new_link->next = state->list_head;
	state->list_head = new_link;
	state->count++;
}

/*
** Handles a directory entry when the GX_ALLDIRS flag is set,
** performing a recursive search with finddir.
*/
static void	process_entry_all_dirs(t_glob_state *state, char *name)
{
	t_finddir_args	args;
	char			*subdir;
	t_list			*dirlist;
	int				ndirs;

	subdir = sh_makepath(state->dir, name, MP_RMDOT);
	if (testdir(subdir) == 0)
	{
		args.pat = state->pat;
		args.sdir = subdir;
		args.flags = (state->flags & ~GX_ADDCURDIR);
		args.ep = &dirlist;
		args.np = &ndirs;
		dirlist = finddir(args);
		if (ndirs)
		{
			args.ep->next = state->list_head;
			state->list_head = dirlist;
			state->count += ndirs;
		}
	}
	add_match_to_list(state, ft_strdup(subdir));
}

/*
** Handles a normal directory entry, matching it against the pattern.
*/
static void	process_entry_normal(t_glob_state *state, char *name)
{
	if (name[0] == '.' && state->pat[0] != '.')
		return ;
	if (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0)
		if (!ft_strchr(state->pat, '.'))
			return ;
	if (!glob_match(state->pat, name))
		add_match_to_list(state, ft_strdup(name));
}

/*
** Opens the directory specified in the state, iterates through its
** entries, and processes each one according to the flags.
*/
static int	read_and_process_dir(t_glob_state *state)
{
	DIR				*d;
	struct dirent	*dp;

	d = opendir(state->dir);
	if (!d)
		return (0);
	while (1)
	{
		dp = readdir(d);
		if (!dp)
			break ;
		if (state->flags & GX_ALLDIRS)
			process_entry_all_dirs(state, dp->d_name);
		else
			process_entry_normal(state, dp->d_name);
	}
	closedir(d);
	return (1);
}

/*
** Allocates a string vector and populates it with the content
** from the linked list of matches.
*/
static char	**convert_list_to_vector(t_list *head, unsigned int count)
{
	char			**vector;
	t_list			*current;
	unsigned int	i;

	vector = mem_malloc((count + 1) * sizeof(char *));
	current = head;
	i = 0;
	while (i < count)
	{
		vector[i] = current->content;
		current = current->next;
		i++;
	}
	vector[count] = NULL;
	return (vector);
}

char	**glob_vector(char *pat, char *dir, int flags)
{
	t_glob_state	state;
	int				add_current;
	struct stat		finfo;
	char			*fullpath;

	init_glob_state(&state, pat, dir, flags);
	if (!pat || !*pat)
	{
		if (testdir(dir) >= 0)
			add_match_to_list(&state, ft_strdup(""));
	}
	else if (!glob_pattern(pat))
	{
		if (testdir(dir) >= 0)
		{
			fullpath = sh_makepath(dir, pat, 0);
			if (lstat(fullpath, &finfo) != 0)
				add_match_to_list(&state, ft_strdup(pat));
		}
	}
	else
		read_and_process_dir(&state);
	add_current = ((flags & (GX_ALLDIRS | GX_ADDCURDIR))
			== (GX_ALLDIRS | GX_ADDCURDIR));
	if (add_current)
		add_match_to_list(&state, ft_strdup(dir));
	return (convert_list_to_vector(state.list_head, state.count));
}
