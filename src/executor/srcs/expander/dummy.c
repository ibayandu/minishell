#include "token.h"
#include "makers.h"
#include "utils.h"

#define DEFAULT_INITIAL_ARRAY_SIZE 112
#define DEFAULT_ARRAY_SIZE 128
#define UNQUOTED 0
#define PARTIALLY_QUOTED 1
#define WHOLLY_QUOTED 2

typedef struct s_variable
{
	char				*name;			/* Symbol that the user types. */
	char				*value;			/* Value that is returned. */
	char				*exportstr;		/* String for the environment. */
	struct s_variable	*(*dynamic_value)(struct s_variable *);	/* Function called to return a `dynamic' value for a variable, like $SECONDS or $RANDOM. */
	struct s_variable	*(*assign_func)(struct s_variable *, char *, long, char *); /* Function called when this `special variable' is assigned a value in bind_variable. */
	int					attributes;		/* export, readonly, array, invisible... */
	int					context;			/* Which context this variable belongs to. */

} t_variable;

typedef struct s_bucket
{
	struct s_bucket	*next;	/* Link to next hashed key in this bucket. */
	char			*key;			/* What we look up. */
	void			*data;			/* What we really want. */
	unsigned int	khash;		/* What key hashes to */
	int				times_found;		/* Number of times this item has been found. */

} t_bucket;

typedef struct s_hash_table
{
	t_bucket	**bucket_array;	/* Where the data is kept. */
	int 		nbuckets;			/* How many buckets does this table have. */
	int 		nentries;			/* How many entries does this table have. */

} t_hash_table;

typedef struct s_context
{
	char				*name;		/* empty or NULL means global context */
	int					scope;		/* 0 means global context */
	int					flags;
	struct s_context	*up;	/* previous function calls */
	struct s_context	*down;	/* down towards global context */
	t_hash_table 		*table;		/* variables at this scope */

} t_context;

t_context *shell_variables = NULL;
t_word_list *subst_assign_varlist = NULL;
t_word_list *garglist = NULL;
int place_keywords_in_env = 0;
int tempenv_assign_error = 0;
int disallow_filename_globbing = 0;
int expand_no_split_dollar_star = 0;
int expanding_redir;
int subshell_environment;
t_hash_table *temporary_env = NULL;
t_hash_table *last_table_searched;
int assigning_in_environment;
int executing_builtin = 0;
t_context *global_variables = NULL;

t_word	*copy_word(t_word *w)
{
  t_word	*new_word;

  new_word = make_bare_word(w->word);
  new_word->flags = w->flags;
  return (new_word);
}

t_word_list	*copy_word_list(t_word_list *list)
{
	t_word_list	*new_list;
	t_word_list	*tl;

	new_list = NULL;
	tl = NULL;
	while (list)
	{
		if (new_list == 0)
			new_list = tl = make_word_list(copy_word (list->word), new_list);
		else
		{
			tl->next = make_word_list(copy_word (list->word), NULL);
			tl = tl->next;
		}
		list = list->next;
	}
	return (new_list);
}

static t_word_list	*separate_out_assignments(t_word_list *tlist)
{
	t_word_list *vp;
	t_word_list	*lp;
	t_word_list	*tp;

	if (!tlist)
		return (NULL);
	if (subst_assign_varlist)
		dispose_words (subst_assign_varlist);	/* Clean up after previous error */
	subst_assign_varlist = NULL;
	vp = lp = tlist;

	/* Separate out variable assignments at the start of the command.
		Loop invariant: vp->next == lp
		Loop postcondition:
	lp = list of words left after assignment statements skipped
	tlist = original list of words
	*/
	while (lp && (lp->word->flags & F_ASSIGNMENT))
	{
		vp = lp;
		lp = lp->next;
	}

	/* If lp != tlist, we have some initial assignment statements.
		We make SUBST_ASSIGN_VARLIST point to the list of assignment
		words and TLIST point to the remaining words.  */
	if (lp != tlist)
	{
		subst_assign_varlist = tlist;
		vp->next =NULL;	/* terminate variable list */
		tlist = lp;			/* remainder of word list */
	}

	/* vp == end of variable list */
	/* tlist == remainder of original word list without variable assignments */
	if (!tlist)
	/* All the words in tlist were assignment statements */
		return (NULL);

	/* If the -k option is in effect, we need to go through the remaining
		words, separate out the assignment words, and place them on
		SUBST_ASSIGN_VARLIST. */
	if (place_keywords_in_env)
	{
		/* tp == running pointer into tlist */

		tp = tlist;
		lp = tlist->next;

		/* Loop Invariant: tp->next == lp */
		/* Loop postcondition: tlist == word list without assignment statements */
		while (lp)
		{
			if (lp->word->flags & F_ASSIGNMENT)
			{
				/* Found an assignment statement, add this word to end of
				subst_assign_varlist (vp). */
				if (!subst_assign_varlist)
					subst_assign_varlist = vp = lp;
				else
				{
					vp->next = lp;
					vp = lp;
				}

				/* Remove the word pointed to by LP from TLIST. */
				tp->next = lp->next;
				lp->next = NULL;
				lp = tp->next;
			}
			else
			{
				tp = lp;
				lp = lp->next;
			}
		}
	}
	return (tlist);
}

int	assignment(const char *string, int flags)
{
	unsigned char	c;
	int	newi;
	int	indx;

	c = string[indx = 0];

	if (legal_variable_starter (c) == 0)
		return (0);
	while (c = string[indx])
	{
		/* The following is safe.  Note that '=' at the start of a word
		is not an assignment statement. */
		if (c == '=')
			return (indx);
		/* Check for `+=' */
		if (c == '+' && string[indx+1] == '=')
			return (indx + 1);
		/* Variable names in assignment statements may contain only letters,
		digits, and `_'. */
		if (legal_variable_char (c) == 0)
			return (0);
		indx++;
	}
	return (0);
}

char	*string_list_internal(t_word_list *list, char *sep)
{
	t_word_list	*t;
	char		*result;
	char		*r;
	size_t		word_len;
	size_t		sep_len;
	size_t		result_size;

	if (!list)
		return (NULL);
	/* Short-circuit quickly if we don't need to separate anything. */
	if (!list->next)
		return (ft_strdup(list->word->word));
	/* This is nearly always called with either sep[0] == 0 or sep[1] == 0. */
	sep_len = ft_strlen(sep);
	result_size = 0;
	t = list;
	while (t)
	{
		if (t != list)
			result_size += sep_len;
		result_size += ft_strlen(t->word->word);
		t = t->next;
	}
	result = ft_malloc(result_size + 1);
	r = result;
	t = list;
	while (t)
	{
		if (t != list && sep_len)
		{
			if (sep_len > 1)
			{
				ft_memcpy(r, sep, sep_len);
				r += sep_len;
			}
			else
				*r++ = sep[0];
		}
		word_len = ft_strlen(t->word->word);
		ft_memcpy(r, t->word->word, word_len);
		r += word_len;
		t = t->next;
	}
	*r = '\0';
	return (result);
}

char	*string_list(t_word_list *list)
{
  return (string_list_internal(list, " "));
}

unsigned int	hash_string(const char *s)
{
	unsigned int	i;

	i = FNV_OFFSET;
	while (*s)
	{
		/* FNV-1a has the XOR first, traditional FNV-1 has the multiply first */

		/* was i *= FNV_PRIME */
		i += (i<<1) + (i<<4) + (i<<7) + (i<<8) + (i<<24);
		i ^= *s;
		s++;
	}
	return i;
}

static void	hash_rehash (t_hash_table *table, int nsize)
{
	int	osize;
	int	i;
	int	j;
	t_bucket	**old_bucket_array;
	t_bucket	*item;
	t_bucket	*next;

	if (!table || nsize == table->nbuckets)
		return ;
	osize = table->nbuckets;
	old_bucket_array = table->bucket_array;
	table->nbuckets = nsize;
	table->bucket_array = ft_malloc (table->nbuckets * sizeof (t_bucket *));
	i = -1;
	while (++i < table->nbuckets)
		table->bucket_array[i] = NULL;
	j = -1;
	while (++j < osize)
	{
		item = old_bucket_array[j];
		while (item)
		{
			next = item->next;
			i = item->khash & (table->nbuckets - 1);
			item->next = table->bucket_array[i];
			table->bucket_array[i] = item;
			item = next;
		}
	}
}

static void	hash_grow(t_hash_table *table)
{
	int	nsize;

	nsize = table->nbuckets * HASH_REHASH_MULTIPLIER;
	if (nsize > 0)		/* overflow */
		hash_rehash (table, nsize);
}

t_bucket	*hash_search (const char *string, t_hash_table *table, int flags)
{
	t_bucket		*list;
	int				bucket;
	unsigned int	hv;
	int				entries;

	if (table)
		entries = table->nentries;
	else
		entries = 0;
	if (!table || ((flags & HASH_CREATE) == 0 && entries == 0))
		return (NULL);
	hv = hash_string(string);
	bucket = hv & (table->nbuckets - 1);
	if (table->bucket_array)
		list = table->bucket_array[bucket];
	else
		list = NULL;
	while (list)
	{
		/* This is the comparison function */
		if (hv == list->khash && (ft_strncmp(list->key, string, ft_strlen(string)) == 0))
		{
			list->times_found++;
			return (list);
		}
		list = list->next;
	}

	if (flags & HASH_CREATE)
	{
		if (table->nentries >= table->nbuckets * 2)
		{
			hash_grow (table);
			hv = hash_string(string);
			bucket = hv & (table->nbuckets - 1);
		}
		list = ft_malloc (sizeof (t_bucket));
		list->next = table->bucket_array[bucket];
		table->bucket_array[bucket] = list;
		list->data = NULL;
		list->key = string;
		list->khash = hv;
		list->times_found = 0;
		table->nentries++;
		return (list);
	}
	return (NULL);
}

static t_variable	*hash_lookup(const char *name, t_hash_table *hashed_vars)
{
	t_bucket	*bucket;

	bucket = hash_search(name, hashed_vars, 0);
	/* If we find the name in HASHED_VARS, set LAST_TABLE_SEARCHED to that
		table. */
	if (bucket)
		last_table_searched = hashed_vars;
	if (bucket)
		return (bucket->data);
	return (NULL);
}

t_variable	*var_lookup(const char *name, t_context *vcontext)
{
	t_context	*vc;
	t_variable	*v;

	v = NULL;
	vc = vcontext;
	while (vc)
	{
		if (v = hash_lookup (name, vc->table))
			break ;
		vc = vc->down;
	}
	return (v);
}

t_variable	*find_variable_internal(const char *name, int flags)
{
	t_variable	*var;
	int			search_tempenv;
	int			force_tempenv;
	t_context	*vc;

	var = NULL;
	force_tempenv = (flags & FV_FORCETEMPENV);

	/* If explicitly requested, first look in the temporary environment for
		the variable.  This allows constructs such as "foo=x eval 'echo $foo'"
		to get the `exported' value of $foo.  This happens if we are executing
		a function or builtin, or if we are looking up a variable in a
		"subshell environment". */
	search_tempenv = force_tempenv || (expanding_redir == 0 && subshell_environment);

	if (search_tempenv && temporary_env)
		var = hash_lookup (name, temporary_env);

	if (!var)
	{
		if ((flags & FV_SKIPINVISIBLE) == 0)
			var = var_lookup (name, shell_variables);
		else
		{
			/* essentially var_lookup expanded inline so we can check for
				att_invisible */
			vc = shell_variables;
			while (vc)
			{
				var = hash_lookup (name, vc->table);
				if (var && var->attributes & 0x0001000)
					var = 0;
				if (var)
					break ;
				vc = vc->down;
			}
		}
	}
	if (!var)
		return (NULL);
	return (var->dynamic_value ? (*(var->dynamic_value)) (var) : var);
}

t_variable	*find_global_variable_noref(const char *name)
{
	t_variable	*var;

	var = var_lookup (name, global_variables);
	if (!var == 0)
		return (NULL);
	return (var->dynamic_value ? (*(var->dynamic_value)) (var) : var);
}

t_variable	*find_variable_nameref(t_variable *v)
{
	int			level;
	int			flags;
	char		*newname;
	t_variable	*orig;
	t_variable	*oldv;

	level = 0;
	orig = v;
	while (v && v->attributes & 0x0000800)
	{
		level++;
		if (level > NAMEREF_MAX)
			return (NULL);
		newname = v->value;
		if (newname == 0 || *newname == '\0')
			return (NULL);
		oldv = v;
		flags = 0;
		if (expanding_redir == 0 && (assigning_in_environment || executing_builtin))
			flags |= FV_FORCETEMPENV;
		/* We don't handle array subscripts here. */
		v = find_variable_internal (newname, flags);
		if (v == orig || v == oldv)
		{
			internal_warning (_("%s: circular name reference"), orig->name);
			if (variable_context && v->context)
				return (find_global_variable_noref (v->name));
			else
				return (NULL);
		}
	}
	return v;
}

/* Look up the variable entry named NAME.  Returns the entry or NULL. */
t_variable	*find_variable (const char *name)
{
	t_variable	*v;
	int			flags;

	last_table_searched = 0;
	flags = 0;
	if (expanding_redir == 0 && (assigning_in_environment || executing_builtin))
		flags |= FV_FORCETEMPENV;
	v = find_variable_internal (name, flags);
	if (v && v->attributes & 0x0000800)
		v = find_variable_nameref (v);
	return v;
}

static t_word	*param_expand (char *string, int *sindex, int quoted, int *expanded_something, int *quoted_dollar_at_p, int *had_quoted_null_p, int pflags)
{
	char			*temp;
	char			*temp1;
	char			uerror[3];
	char			*savecmd;
	int				zindex;
	int				t_index;
	int				expok;
	int				eflag;
	unsigned char	c;
	long			number;
	t_variable		*var;
	t_word_list		*list;
	t_word_list		*l;
	t_word			*tdesc;
	t_word			*ret;
	int				tflag;
	int				nullarg;

	zindex = *sindex;
	c = string[++zindex];

	temp = NULL;
	ret = NULL;
	tdesc = NULL;
	tflag = 0;

	/* Do simple cases first. Switch on what follows '$'. */
	switch (c)
	{
		/* $? -- return value of the last synchronous command. */
		case '?':
			temp = ft_itoa(last_command_exit_value);
			break ;
		default:
			/* Find the variable in VARIABLE_LIST. */
			temp = NULL;
			t_index = zindex;
			while ((c = string[zindex]) && ft_isalnum(c))
				zindex++;
			if (zindex > t_index)
				temp1 = ft_substr(string, t_index, zindex);
			else
				temp1 = NULL;

			/* If this isn't a variable name, then just output the `$'. */
			if (!temp1 || !*temp1)
			{
				temp = ft_malloc (2);
				temp[0] = '$';
				temp[1] = '\0';
				if (expanded_something)
					*expanded_something = 0;
				goto return0;
			}
			/* If the variable exists, return its value cell. */
			var = find_variable (temp1);

			if (var && invisible_p (var) == 0 && var_isset (var))
			{
				temp = value_cell (var);
				temp = (*temp && (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES))) ? quote_string (temp) : ((pflags & PF_ASSIGNRHS) ? quote_rhs (temp) : quote_escapes (temp));
				goto return0;
			}
			else if (var && (invisible_p (var) || var_isset (var) == 0))
				temp = NULL;
			else if ((var = find_variable_last_nameref (temp1, 0)) && var_isset (var) && invisible_p (var) == 0)
			{
				temp = nameref_cell (var);
				/* y=2 ; typeset -n x=y; echo $x is not the same as echo $2 in ksh */
				if (temp && *temp && legal_identifier (temp) == 0)
				{
					set_exit_status (EXECUTION_FAILURE);
					report_error (_("%s: invalid variable name for name reference"), temp);
					return (&expand_wdesc_error);	/* XXX */
				}
				else
					temp = NULL;
			}
			temp = NULL;

			unbound_variable:
			if (unbound_vars_is_error)
			{
				set_exit_status (EXECUTION_FAILURE);
				err_unboundvar (temp1);
			}
			else
				goto return0;
			set_exit_status (EXECUTION_FAILURE);
			return ((unbound_vars_is_error && interactive_shell == 0) ? &expand_wdesc_fatal : &expand_wdesc_error);
	}
	if (string[zindex])
		zindex++;
	return0:
	*sindex = zindex;
	if (ret == 0)
	{
		ret = alloc_word_desc();
		ret->flags = tflag;
		ret->word = temp;
	}
	return ret;
}

static char	*expand_string_if_necessary(char *string, int quoted, t_word_list *(*func)(char *, int))
{
	t_word_list	*list;
	size_t		slen;
	int			i;
	int			saw_quote;
	char			*ret;

	if (MB_CUR_MAX > 1)
		slen = ft_strlen(string);
	else
		slen = 0;
	i = 0;
	saw_quote = 0;
	while (string[i])
	{
		if (string[i] == '$'
			|| string[i] == '<'
			|| string[i] == '>'
			|| string[i] == '\001')
			break ;
		else if (string[i] == '\'' || string[i] == '"')
			saw_quote = 1;
		i += ft_mbrlen(string + i, slen - i);
	}
	if (string[i])
	{
		list = func(string, quoted);
		if (list)
			ret = string_list(list);
		else
			ret = NULL;
	}
	else if (saw_quote)
		ret = string_quote_removal(string);
	else
		ret = ft_strdup(string);
	return ret;
}

char	*sub_append_string(char *source, char *target, size_t *indx, size_t *size)
{
	size_t	n;
	size_t	srclen;

	if (source)
	{
		srclen = ft_strlen(source);
		if (srclen >= (*size - *indx))
		{
			n = srclen + *indx;
			n = (n + DEFAULT_ARRAY_SIZE) - (n % DEFAULT_ARRAY_SIZE);
			target = ft_realloc(target, (*size = n));
		}
		ft_memcpy(target + *indx, source, srclen);
		*indx += srclen;
		target[*indx] = '\0';
	}
	return (target);
}

static t_word_list	*expand_word_internal(t_word *word, int quoted, int isexp, int *expanded_something)
{
	t_word_list		*list;
	t_word			*tword;
	char			*istring;
	size_t			istring_size;
	size_t			istring_index;
	char			*temp;
	char			*temp1;
	char			*string;
	size_t			string_size;
	int				sindex;
	int				quoted_dollar_at;
	int				quoted_state;
	int				had_quoted_null;
	int				has_quoted_ifs;
	int				split_on_spaces;
	int				local_expanded;
	int				tflag;
	int				pflags;
	int				assignoff;
	unsigned char	c;
	int				t_index;
	char			twochars[2];

	istring = ft_malloc(istring_size = DEFAULT_INITIAL_ARRAY_SIZE);
	istring_index = 0;
	istring[istring_index] = '\0';
	quoted_dollar_at = 0;
	had_quoted_null = 0;
	has_quoted_ifs = 0;
	split_on_spaces = 0;
	quoted_state = UNQUOTED;

	string = word->word;
	if (!string)
		goto finished_with_string;

	if (MB_CUR_MAX > 1)
		string_size = ft_strlen(string);
	else
		string_size = 1;

	assignoff = -1;

	/* Begin the expansion. */

	sindex = 0;
	while (1)
	{
		c = string[sindex];

		/* Case on top-level character. */
		switch (c)
		{
			case '\0':
				goto finished_with_string;
			case '\001':
				sindex++;
				if (MB_CUR_MAX > 1 && string[sindex])
					add_mbqchar_body(temp, string, sindex, string_size);
				else
				{
					temp = ft_malloc (3);
					temp[0] = '\001';
					temp[1] = c = string[sindex];
					temp[2] = '\0';
				}

				dollar_add_string:
				if (string[sindex])
				sindex++;

				add_string:
				if (temp)
				{
					istring = sub_append_string(temp, istring, &istring_index, &istring_size);
					temp = (char *)0;
				}
				break ;
			case '=':
				/* Posix.2 section 3.6.1 says that tildes following `=' in words
					which are not assignment statements are not expanded.  If the
					shell isn't in posix mode, though, we perform tilde expansion
					on `likely candidate' unquoted assignment statements (flags
					include W_ASSIGNMENT but not W_QUOTED).  A likely candidate
					contains an unquoted :~ or =~.  Something to think about: we
					now have a flag that says  to perform tilde expansion on arguments
					to `assignment builtins' like declare and export that look like
					assignment statements.  We now do tilde expansion on such words
					even in POSIX mode. */
				if (word->flags & (W_ASSIGNRHS|W_NOTILDE))
				{
					if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
						goto add_ifs_character;
					else
						goto add_character;
				}
				/* If we're not in posix mode or forcing assignment-statement tilde
					expansion, note where the first `=' appears in the word and prepare
					to do tilde expansion following the first `='. We have to keep
					track of the first `=' (using assignoff) to avoid being confused
					by an `=' in the rhs of the assignment statement. */
				if ((word->flags & W_ASSIGNMENT) && assignoff == -1 && sindex > 0)
					assignoff = sindex;
				if (word->flags & W_ASSIGNARG)
					word->flags |= W_ASSIGNRHS;		/* affects $@ */
				if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
				{
					has_quoted_ifs++;
					goto add_ifs_character;
				}
				else
					goto add_character;

			case '$':
				if (expanded_something)
					*expanded_something = 1;
				local_expanded = 1;

				if (word->flags & W_NOCOMSUB)
					pflags = PF_NOCOMSUB;
				else
					pflags = 0;
				if (word->flags & W_NOSPLIT2)
					pflags |= PF_NOSPLIT2;
				if (word->flags & W_ASSIGNRHS)
					pflags |= PF_ASSIGNRHS;
				if (word->flags & W_COMPLETE)
					pflags |= PF_COMPLETE;
				tword = param_expand (string, &sindex, quoted, expanded_something,
							&temp_has_dollar_at, &quoted_dollar_at,
							&had_quoted_null, pflags);
				split_on_spaces += (tword->flags & W_SPLITSPACE);
				if (tword == &expand_wdesc_error || tword == &expand_wdesc_fatal)
					return ((tword == &expand_wdesc_error) ? &expand_word_error
										: &expand_word_fatal);
				if (tword && (tword->flags & W_HASQUOTEDNULL))
				had_quoted_null = 1;		/* note for later */
				if (tword && (tword->flags & W_SAWQUOTEDNULL))
				had_quoted_null = 1;		/* XXX */
				if (tword)
					temp = tword->word;
				else
					temp = NULL;
				/* Kill quoted nulls; we will add them back at the end of
					expand_word_internal if nothing else in the string */
				if (had_quoted_null && temp && QUOTED_NULL (temp))
					temp = NULL;
				goto add_string;
				break ;
			case '"':
				if ((quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)) && ((quoted & Q_ARITH) == 0))
					goto add_character;
				t_index = ++sindex;
				temp = string_extract_double_quoted (string, &sindex, (word->flags & W_COMPLETE) ? SX_COMPLETE : 0);

				/* If the quotes surrounded the entire string, then the
					whole word was quoted. */
				quoted_state = (t_index == 1 && string[sindex] == '\0')
						? WHOLLY_QUOTED
						: PARTIALLY_QUOTED;

				if (temp && *temp)
				{
					tword = alloc_word_desc ();
					tword->word = temp;

					if (word->flags & W_ASSIGNARG)
						tword->flags |= word->flags & (W_ASSIGNARG|W_ASSIGNRHS); /* affects $@ */
					if (word->flags & W_COMPLETE)
						tword->flags |= W_COMPLETE;	/* for command substitutions */
					if (word->flags & W_NOCOMSUB)
						tword->flags |= W_NOCOMSUB;
					if (word->flags & W_NOPROCSUB)
						tword->flags |= W_NOPROCSUB;
					if (word->flags & W_ASSIGNRHS)
						tword->flags |= W_ASSIGNRHS;
					temp = NULL;
					/* Need to get W_HASQUOTEDNULL flag through this function. */
					list = expand_word_internal (tword, Q_DOUBLE_QUOTES|(quoted&Q_ARITH), 0, (int *)NULL);
					if (list == &expand_word_error || list == &expand_word_fatal)
						return list;
					/* If this list comes back with a quoted null from expansion,
					we have either "$x" or "$@" with $1 == ''. In either case,
					we need to make sure we add a quoted null argument and
					disable the special handling that "$@" gets. */
					if (list && list->word && list->next == 0 && (list->word->flags & W_HASQUOTEDNULL))
						had_quoted_null = 1;
					if (list)
						dequote_list (list);
				}
				else
				{
					list = NULL;
					had_quoted_null = 1;
				}

				/* The code above *might* return a list (consider the case of "$@",
					where it returns "$1", "$2", etc.).  We can't throw away the
					rest of the list, and we have to make sure each word gets added
					as quoted.  We test on tresult->next:  if it is non-NULL, we
					quote the whole list, save it to a string with string_list, and
					add that string. We don't need to quote the results of this
					(and it would be wrong, since that would quote the separators
					as well), so we go directly to add_string. */
				if (list)
				{
					if (list->next)
					{
						temp =  string_list(quote_list (list));
						goto add_string;
					}
					else
					{
						temp = ft_strdup(list->word->word);
						tflag = list->word->flags;
						/* If the string is not a quoted null string, we want
							to remove any embedded unquoted CTLNUL characters.
							We do not want to turn quoted null strings back into
							the empty string, though.  We do this because we
							want to remove any quoted nulls from expansions that
							contain other characters.  For example, if we have
							x"$*"y or "x$*y" and there are no positional parameters,
							the $* should expand into nothing. */
						/* We use the W_HASQUOTEDNULL flag to differentiate the
							cases:  a quoted null character as above and when
							CTLNUL is contained in the (non-null) expansion
							of some variable.  We use the had_quoted_null flag to
							pass the value through this function to its caller. */
						if ((tflag & W_HASQUOTEDNULL) && QUOTED_NULL (temp) == 0)
							remove_quoted_nulls (temp);	/* XXX */
					}
				}
				else
					temp = NULL;
				if (temp == 0 && quoted_state == PARTIALLY_QUOTED)
					had_quoted_null = 1;

				/* We do not want to add quoted nulls to strings that are only
					partially quoted; we can throw them away.  The exception to
					this is when we are going to be performing word splitting,
					since we have to preserve a null argument if the next character
					will cause word splitting. */
				if (temp == 0 && quoted_state == PARTIALLY_QUOTED && quoted == 0 && (word->flags & (W_NOSPLIT|W_EXPANDRHS|W_ASSIGNRHS)) == W_EXPANDRHS)
				{
					c = CTLNUL;
					sindex--;
					had_quoted_null = 1;
					goto add_character;
				}
				if (temp == 0 && quoted_state == PARTIALLY_QUOTED && (word->flags & (W_NOSPLIT|W_NOSPLIT2)))
					continue ;
			add_quoted_string:
				if (temp)
				{
					temp1 = temp;
					temp = quote_string (temp);
					free (temp1);
					goto add_string;
				}
				else
				{
					/* Add NULL arg. */
					c = CTLNUL;
					sindex--;		/* add_character: label increments sindex */
					had_quoted_null = 1;	/* note for later */
					goto add_character;
				}

			case '\'':
				if ((quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)))
					goto add_character;

				t_index = ++sindex;
				temp = string_extract_single_quoted (string, &sindex, 0);

				/* If the entire STRING was surrounded by single quotes,
					then the string is wholly quoted. */
				quoted_state = (t_index == 1 && string[sindex] == '\0')
						? WHOLLY_QUOTED
						: PARTIALLY_QUOTED;

				/* If all we had was '', it is a null expansion. */
				if (*temp == '\0')
					temp = NULL;
				else
					remove_quoted_escapes (temp);

				if (temp == 0 && quoted_state == PARTIALLY_QUOTED)
					had_quoted_null = 1;

				/* We do not want to add quoted nulls to strings that are only
					partially quoted; such nulls are discarded.  See above for the
					exception, which is when the string is going to be split.
					Posix interp 888/1129 */
				if (temp == 0 && quoted_state == PARTIALLY_QUOTED && quoted == 0 && (word->flags & (W_NOSPLIT|W_EXPANDRHS|W_ASSIGNRHS)) == W_EXPANDRHS)
				{
					c = CTLNUL;
					sindex--;
					goto add_character;
				}

				if (temp == 0 && (quoted_state == PARTIALLY_QUOTED) && (word->flags & (W_NOSPLIT|W_NOSPLIT2)))
					continue;

				/* If we have a quoted null expansion, add a quoted NULL to istring. */
				if (temp == 0)
				{
					c = CTLNUL;
					sindex--;		/* add_character: label increments sindex */
					goto add_character;
				}
				else
					goto add_quoted_string;
			case ' ':
				/* If we are in a context where the word is not going to be split, but
					we need to account for $@ and $* producing one word for each
					positional parameter, add quoted spaces so the spaces in the
					expansion of "$@", if any, behave correctly. We still may need to
					split if we are expanding the rhs of a word expansion. */
				if (ifs_is_null || split_on_spaces || ((word->flags & (W_NOSPLIT|W_NOSPLIT2|W_ASSIGNRHS)) && (word->flags & W_EXPANDRHS) == 0))
				{
					if (string[sindex])
						sindex++;
					twochars[0] = CTLESC;
					twochars[1] = c;
					goto add_twochars;
				}
				/* FALLTHROUGH */
			default:
				/* This is the fix for " $@ " */
				add_ifs_character:
				if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) || (isexp == 0 && isifs (c) && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0))
				{
					if ((quoted&(Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) == 0)
						has_quoted_ifs++;
					add_quoted_character:
					if (string[sindex])	/* from old goto dollar_add_string */
						sindex++;
					if (c == 0)
					{
						c = CTLNUL;
						goto add_character;
					}
					else
					{
						if (MB_CUR_MAX > 1)
							add_mbqchar_body(temp, string, --sindex, string_size);
						else
						{
							twochars[0] = '\001';
							twochars[1] = c;
							goto add_twochars;
						}
					}
				}
				SADD_MBCHAR (temp, string, sindex, string_size);
				add_character:
				RESIZE_MALLOCED_BUFFER (istring, istring_index, 1, istring_size,
							DEFAULT_ARRAY_SIZE);
				istring[istring_index++] = c;
				istring[istring_index] = '\0';
				sindex++;
		}
	}

	finished_with_string:
	/* OK, we're ready to return.  If we have a quoted string, and
		quoted_dollar_at is not set, we do no splitting at all; otherwise
		we split on ' '.  The routines that call this will handle what to
		do if nothing has been expanded. */

	/* Partially and wholly quoted strings which expand to the empty
		string are retained as an empty arguments.  Unquoted strings
		which expand to the empty string are discarded.  The single
		exception is the case of expanding "$@" when there are no
		positional parameters.  In that case, we discard the expansion. */

	/* Because of how the code that handles "" and '' in partially
		quoted strings works, we need to make ISTRING into a QUOTED_NULL
		if we saw quoting characters, but the expansion was empty.
		"" and '' are tossed away before we get to this point when
		processing partially quoted strings.  This makes "" and $xxx""
		equivalent when xxx is unset.  We also look to see whether we
		saw a quoted null from a ${} expansion and add one back if we
		need to. */

	/* If we expand to nothing and there were no single or double quotes
		in the word, we throw it away.  Otherwise, we return a NULL word.
		The single exception is for $@ surrounded by double quotes when
		there are no positional parameters.  In that case, we also throw
		the word away. */

	if (*istring == '\0')
	{
		if (had_quoted_null || (quoted_dollar_at == 0 && quoted_state == PARTIALLY_QUOTED))
		{
			istring[0] = CTLNUL;
			istring[1] = '\0';
			tword = alloc_word_desc ();
			tword->word = istring;
			istring = 0;		/* avoid later free() */
			tword->flags |= W_HASQUOTEDNULL;		/* XXX */
			list = make_word_list (tword, NULL);
			if (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES))
				tword->flags |= W_QUOTED;
		}
		/* According to sh, ksh, and Posix.2, if a word expands into nothing
		and a double-quoted "$@" appears anywhere in it, then the entire
		word is removed. */
		else if (quoted_state == UNQUOTED || quoted_dollar_at)
			list = NULL;
		else
			list = NULL;
	}
	else if (word->flags & W_NOSPLIT)
	{
		tword = alloc_word_desc ();
		tword->word = istring;
		if (had_quoted_null && QUOTED_NULL (istring))
			tword->flags |= W_HASQUOTEDNULL;
		istring = 0;		/* avoid later free() */
		if (word->flags & W_ASSIGNMENT)
			tword->flags |= W_ASSIGNMENT;	/* XXX */
		if (word->flags & W_COMPASSIGN)
			tword->flags |= W_COMPASSIGN;	/* XXX */
		if (word->flags & W_NOGLOB)
			tword->flags |= W_NOGLOB;	/* XXX */
		if (word->flags & W_NOBRACE)
			tword->flags |= W_NOBRACE;	/* XXX */
		if (word->flags & W_ARRAYREF)
			tword->flags |= W_ARRAYREF;
		if (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES))
			tword->flags |= W_QUOTED;
		list = make_word_list (tword, NULL);
	}
	else if (word->flags & W_ASSIGNRHS)
	{
		list = list_string (istring, "", quoted);
		tword = list->word;
		if (had_quoted_null && QUOTED_NULL (istring))
			tword->flags |= W_HASQUOTEDNULL;
		istring = 0;			/* avoid later free() */
		goto set_word_flags;
	}
	else
	{
		char *ifs_chars;
		ifs_chars = (quoted_dollar_at || has_dollar_at) ? ifs_value : (char *)NULL;
		/* If we have $@, we need to split the results no matter what.  If
		IFS is unset or NULL, string_list_dollar_at has separated the
		positional parameters with a space, so we split on space (we have
		set ifs_chars to " \t\n" above if ifs is unset).  If IFS is set,
		string_list_dollar_at has separated the positional parameters
		with the first character of $IFS, so we split on $IFS.  If
		SPLIT_ON_SPACES is set, we expanded $* (unquoted) with IFS either
		unset or null, and we want to make sure that we split on spaces
		regardless of what else has happened to IFS since the expansion,
		or we expanded "$@" with IFS null and we need to split the positional
		parameters into separate words. */
		if (split_on_spaces)
		{
			/* If IFS is not set, and the word is not quoted, we want to split
				the individual words on $' \t\n'. We rely on previous steps to
				quote the portions of the word that should not be split */
			if (ifs_is_set == 0)
				list = list_string (istring, " \t\n", 1);
			else
				list = list_string (istring, " ", 1);
		}

		/* If we have $@ (has_dollar_at != 0) and we are in a context where we
		don't want to split the result (W_NOSPLIT2), and we are not quoted,
		we have already separated the arguments with the first character of
		$IFS.  In this case, we want to return a list with a single word
		with the separator possibly replaced with a space (it's what other
		shells seem to do).
		quoted_dollar_at is internal to this function and is set if we are
		passed an argument that is unquoted (quoted == 0) but we encounter a
		double-quoted $@ while expanding it. */
		else if (has_dollar_at && quoted_dollar_at == 0 && ifs_chars && quoted == 0 && (word->flags & W_NOSPLIT2))
		{
			tword = alloc_word_desc ();
			/* Only split and rejoin if we have to */
			if (*ifs_chars && *ifs_chars != ' ')
			{
				/* list_string dequotes CTLESCs in the string it's passed, so we
				need it to get the space separation right if space isn't the
				first character in IFS (but is present) and to remove the
				quoting we added back in param_expand(). */
				list = list_string (istring, *ifs_chars ? ifs_chars : " ", 1);
				/* This isn't exactly right in the case where we're expanding
				the RHS of an expansion like ${var-$@} where IFS=: (for
				example). The W_NOSPLIT2 means we do the separation with :;
				the list_string removes the quotes and breaks the string into
				a list, and the string_list rejoins it on spaces. When we
				return, we expect to be able to split the results, but the
				space separation means the right split doesn't happen. */
				tword->word = string_list (list);
			}
			else
				tword->word = istring;
			if (had_quoted_null && QUOTED_NULL (istring))
				tword->flags |= W_HASQUOTEDNULL;	/* XXX */
			istring = 0;			/* avoid later free() */
			goto set_word_flags;
		}
		else if (has_dollar_at && ifs_chars)
			list = list_string (istring, *ifs_chars ? ifs_chars : " ", 1);
		else
		{
			tword = alloc_word_desc ();
			if (expanded_something && *expanded_something == 0 && has_quoted_ifs)
				tword->word = remove_quoted_ifs (istring);
			else
				tword->word = istring;
			if (had_quoted_null && QUOTED_NULL (istring))	/* should check for more than one */
				tword->flags |= W_HASQUOTEDNULL;	/* XXX */
			else if (had_quoted_null)
				tword->flags |= W_SAWQUOTEDNULL;	/* XXX */
			istring = 0;			/* avoid later free() */
			set_word_flags:
			if ((quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)) || (quoted_state == WHOLLY_QUOTED))
				tword->flags |= W_QUOTED;
			if (word->flags & W_ASSIGNMENT)
				tword->flags |= W_ASSIGNMENT;
			if (word->flags & W_COMPASSIGN)
				tword->flags |= W_COMPASSIGN;
			if (word->flags & W_NOGLOB)
				tword->flags |= W_NOGLOB;
			if (word->flags & W_NOBRACE)
				tword->flags |= W_NOBRACE;
			if (word->flags & W_ARRAYREF)
				tword->flags |= W_ARRAYREF;
			list = make_word_list(tword, NULL);
		}
	}
	return (list);
}

static t_word_list	*call_expand_word_internal(t_word *w, int q, int i, int e)
{
	t_word_list	*result;

	result = expand_word_internal (w, q, i, e);
	if (result == &expand_word_error || result == &expand_word_fatal)
	{
		/* By convention, each time this error is returned, w->word has
		already been freed (it sometimes may not be in the fatal case,
		but that doesn't result in a memory leak because we're going
		to exit in most cases). */
		w->word = NULL;
		last_command_exit_value = EXECUTION_FAILURE;
		exp_jump_to_top_level ((result == &expand_word_error) ? DISCARD : FORCE_EOF);
		/* NOTREACHED */
		return (NULL);
	}
	else
		return (result);
}

t_word_list	*expand_string_assignment(char *string, int quoted)
{
	t_word		td;
	t_word_list	*value;

	if (!string || !*string)
		return (NULL);
	expand_no_split_dollar_star = 1;
	td.flags = W_ASSIGNRHS;
	td.flags |= (W_NOGLOB | W_TILDEEXP);
	td.word = ft_strdup(string);
	value = call_expand_word_internal(&td, quoted, 0, (int *)NULL);
	expand_no_split_dollar_star = 0;
	if (value)
	{
		if (value->word)
		{
			remove_quoted_nulls(value->word->word);
			value->word->flags &= ~W_HASQUOTEDNULL;
		}
		dequote_list (value);
	}
	return (value);
}

static int	do_assignment_internal(t_word *word, int expand)
{
	int			offset;
	int			appendop;
	int			assign_list;
	int			aflags;
	int			retval;
	char		*name;
	char		*value;
	char		*temp;
	t_variable	*entry;
	const char	*string;

	if (!word || !word->word)
		return 0;
	appendop = 0;
	assign_list = 0;
	aflags = 0;
	string = word->word;
	offset = assignment(string, 0);
	name = ft_strdup(string);
	value = NULL;

	if (name[offset] == '=')
	{
		if (name[offset - 1] == '+')
		{
			appendop = 1;
			name[offset - 1] = '\0';
		}
		name[offset] = 0;		/* might need this set later */
		temp = name + offset + 1;
		if (expand && temp[0])
			value = expand_string_if_necessary(temp, 0, expand_string_assignment);
		else
			value = ft_strdup(temp);
	}
	if (!value)
		value = ft_strdup("");
	if (echo_command_at_execute)
	{
		if (appendop)
		name[offset - 1] = '+';
		xtrace_print_assignment (name, value, assign_list, 1);
		if (appendop)
			name[offset - 1] = '\0';
	}
	if (appendop)
		aflags |= ASS_APPEND;
	entry = bind_variable (name, value, aflags);
	if (entry)
		stupidly_hack_special_variables (entry->name);	/* might be a nameref */
	else
		stupidly_hack_special_variables (name);
	/* Return 1 if the assignment seems to have been performed correctly. */
	if (entry == 0 || readonly_p (entry))
		retval = 0;		/* assignment failure */
	else if (noassign_p (entry))
	{
		set_exit_status (EXECUTION_FAILURE);
		retval = 1;	/* error status, but not assignment failure */
	}
	else
		retval = 1;
	if (entry && retval != 0 && noassign_p (entry) == 0)
		VUNSETATTR (entry, att_invisible);
	return (retval);
}

int	do_word_assignment(t_word *word, int flags)
{
	return do_assignment_internal(word, 1);
}

static int	do_assignment_statements (t_word_list *varlist, char *command, int is_nullcmd)
{
	t_word_list	*temp_list;
	char		*savecmd;
	int			is_special_builtin;
	int			is_builtin_or_func;
	int			tint;
	int			(*assign_func)(t_word *, int);

	/* If the remainder of the words expand to nothing, Posix.2 requires
		that the variable and environment assignments affect the shell's
		environment (do_word_assignment). */
	if (is_nullcmd)
		assign_func = do_word_assignment
	else
		assing_func = assign_in_env;
	tempenv_assign_error = 0;

	is_builtin_or_func = command && (find_shell_builtin (command) || find_function (command));
	/* Posix says that special builtins exit if a variable assignment error
		occurs in an assignment preceding it. (XXX - this is old -- current Posix
		says that any variable assignment error causes a non-interactive shell
		to exit. See the STRICT_POSIX checks below. */
	is_special_builtin = posixly_correct && command && find_special_builtin (command);

	savecmd = this_command_name;
	for (temp_list = varlist; temp_list; temp_list = temp_list->next)
	{
		this_command_name = NULL;
		assigning_in_environment = is_nullcmd == 0;
		tint = (*assign_func) (temp_list->word, is_builtin_or_func);
		assigning_in_environment = 0;
		this_command_name = savecmd;

		/* Variable assignment errors in non-interactive shells running
		in posix mode cause the shell to exit. */
		if (tint == 0)
	{
		if (is_nullcmd)	/* assignment statement */
		{
			last_command_exit_value = EXECUTION_FAILURE;
	#if defined (STRICT_POSIX)
			if (posixly_correct && interactive_shell == 0)
	#else
			if (posixly_correct && interactive_shell == 0 && executing_command_builtin == 0)
	#endif
			exp_jump_to_top_level (FORCE_EOF);
			else
		exp_jump_to_top_level (DISCARD);
		}
		/* In posix mode, assignment errors in the temporary environment
			cause a non-interactive shell executing a special builtin to
			exit and a non-interactive shell to otherwise jump back to the
			top level. This is what POSIX says to do for variable assignment
			errors, and POSIX says errors in assigning to the temporary
			environment are treated as variable assignment errors.
			(XXX - this is not what current POSIX says - look at the
			STRICT_POSIX defines. */
		else if (posixly_correct)
		{
			last_command_exit_value = EXECUTION_FAILURE;
	#if defined (STRICT_POSIX)
			exp_jump_to_top_level ((interactive_shell == 0) ? FORCE_EOF : DISCARD);
	#else
			if (interactive_shell == 0 && is_special_builtin)
		exp_jump_to_top_level (FORCE_EOF);
			else if (interactive_shell == 0)
		exp_jump_to_top_level (DISCARD);	/* XXX - maybe change later */
			else
		exp_jump_to_top_level (DISCARD);
	#endif
		}
		else
		tempenv_assign_error++;
	}
	}
	return (tempenv_assign_error);
}

static t_word_list	*expand_word_list_internal(t_word_list *list)
{
	t_word_list	*new_list;
	t_word_list	*temp_list;

	tempenv_assign_error = 0;
	if (!list)
		return (NULL);

	new_list = separate_out_assignments(copy_word_list(list));
	garglist = new_list;
	if (new_list == 0)
	{
		if (subst_assign_varlist)
			do_assignment_statements (subst_assign_varlist, NULL, 1);
		subst_assign_varlist = NULL;
		return (NULL);
	}

	/* Begin expanding the words that remain.  The expansions take place on
		things that aren't really variable assignments. */

	/* Perform the `normal' shell expansions: tilde expansion, parameter and
		variable substitution, command substitution, arithmetic expansion,
		and word splitting. */
	new_list = shell_expand_word_list (new_list, WEXP_ALL);
	if (new_list)
	{
		if (disallow_filename_globbing == 0)
			/* Glob expand the word list unless globbing has been disabled. */
			new_list = glob_expand_word_list (new_list, WEXP_ALL);
		else
			/* Dequote the words, because we're not performing globbing. */
			new_list = dequote_list (new_list);
	}
	if (subst_assign_varlist)
	{
		if (new_list && new_list->word)
			do_assignment_statements(subst_assign_varlist, new_list->word->word, !new_list);
		else
			do_assignment_statements(subst_assign_varlist, NULL, !new_list);
		dispose_words (subst_assign_varlist);
		subst_assign_varlist = NULL;
	}
	return (new_list);
}
