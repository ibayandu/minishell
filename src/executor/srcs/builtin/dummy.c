static char **
make_env_array_from_var_list (vars)
     SHELL_VAR **vars;
{
  register int i, list_index;
  register SHELL_VAR *var;
  char **list, *value;

  list = strvec_create ((1 + strvec_len ((char **)vars)));

  for (i = 0, list_index = 0; var = vars[i]; i++)
    {
#if defined (__CYGWIN__)
      /* We don't use the exportstr stuff on Cygwin at all. */
      INVALIDATE_EXPORTSTR (var);
#endif

      /* If the value is generated dynamically, generate it here. */
      if (regen_p (var) && var->dynamic_value)
	{
	  var = (*(var->dynamic_value)) (var);
	  INVALIDATE_EXPORTSTR (var);
	}

      if (var->exportstr)
	value = var->exportstr;
      else if (function_p (var))
	value = named_function_string ((char *)NULL, function_cell (var), 0);
#if defined (ARRAY_VARS)
      else if (array_p (var))
#  if ARRAY_EXPORT
	value = array_to_assign (array_cell (var), 0);
#  else
	continue;	/* XXX array vars cannot yet be exported */
#  endif /* ARRAY_EXPORT */
      else if (assoc_p (var))
#  if ARRAY_EXPORT
	value = assoc_to_assign (assoc_cell (var), 0);
#  else
	continue;	/* XXX associative array vars cannot yet be exported */
#  endif /* ARRAY_EXPORT */
#endif
      else
	value = value_cell (var);

      if (value)
	{
	  /* Gee, I'd like to get away with not using savestring() if we're
	     using the cached exportstr... */
	  list[list_index] = USE_EXPORTSTR ? savestring (value)
					   : mk_env_string (var->name, value, var->attributes);

	  if (USE_EXPORTSTR == 0)
	    SAVE_EXPORTSTR (var, list[list_index]);

	  list_index++;
#undef USE_EXPORTSTR

#if defined (ARRAY_VARS) && defined (ARRAY_EXPORT)
	  if (array_p (var) || assoc_p (var))
	    free (value);
#endif
	}
    }

  list[list_index] = (char *)NULL;
  return (list);
}

static VARLIST *
vlist_alloc (nentries)
     int nentries;
{
  VARLIST  *vlist;

  vlist = (VARLIST *)xmalloc (sizeof (VARLIST));
  vlist->list = (SHELL_VAR **)xmalloc ((nentries + 1) * sizeof (SHELL_VAR *));
  vlist->list_size = nentries;
  vlist->list_len = 0;
  vlist->list[0] = (SHELL_VAR *)NULL;

  return vlist;
}

static VARLIST *
vlist_realloc (vlist, n)
     VARLIST *vlist;
     int n;
{
  if (vlist == 0)
    return (vlist = vlist_alloc (n));
  if (n > vlist->list_size)
    {
      vlist->list_size = n;
      vlist->list = (SHELL_VAR **)xrealloc (vlist->list, (vlist->list_size + 1) * sizeof (SHELL_VAR *));
    }
  return vlist;
}

static void
vlist_add (vlist, var, flags)
     VARLIST *vlist;
     SHELL_VAR *var;
     int flags;
{
  register int i;

  for (i = 0; i < vlist->list_len; i++)
    if (STREQ (var->name, vlist->list[i]->name))
      break;
  if (i < vlist->list_len)
    return;

  if (i >= vlist->list_size)
    vlist = vlist_realloc (vlist, vlist->list_size + 16);

  vlist->list[vlist->list_len++] = var;
  vlist->list[vlist->list_len] = (SHELL_VAR *)NULL;
}

static void
flatten (var_hash_table, func, vlist, flags)
     HASH_TABLE *var_hash_table;
     sh_var_map_func_t *func;
     VARLIST *vlist;
     int flags;
{
  register int i;
  register BUCKET_CONTENTS *tlist;
  int r;
  SHELL_VAR *var;

  if (var_hash_table == 0 || (HASH_ENTRIES (var_hash_table) == 0) || (vlist == 0 && func == 0))
    return;

  for (i = 0; i < var_hash_table->nbuckets; i++)
    {
      for (tlist = hash_items (i, var_hash_table); tlist; tlist = tlist->next)
	{
	  var = (SHELL_VAR *)tlist->data;

	  r = func ? (*func) (var) : 1;
	  if (r && vlist)
	    vlist_add (vlist, var, flags);
	}
    }
}

SHELL_VAR **
map_over (function, vc)
     sh_var_map_func_t *function;
     VAR_CONTEXT *vc;
{
  VAR_CONTEXT *v;
  VARLIST *vlist;
  SHELL_VAR **ret;
  int nentries;

  for (nentries = 0, v = vc; v; v = v->down)
    nentries += HASH_ENTRIES (v->table);

  if (nentries == 0)
    return (SHELL_VAR **)NULL;

  vlist = vlist_alloc (nentries);

  for (v = vc; v; v = v->down)
    flatten (v->table, function, vlist, 0);

  ret = vlist->list;
  free (vlist);
  return ret;
}


static char **
make_var_export_array (vcxt)
     VAR_CONTEXT *vcxt;
{
  char **list;
  SHELL_VAR **vars;

#if 0
  vars = map_over (visible_and_exported, vcxt);
#else
  vars = map_over (export_environment_candidate, vcxt);
#endif

  if (vars == 0)
    return (char **)NULL;

  list = make_env_array_from_var_list (vars);

  free (vars);
  return (list);
}
