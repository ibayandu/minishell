int
assignment (string, flags)
     const char *string;
     int flags;
{
  register unsigned char c;
  register int newi, indx;

  c = string[indx = 0];

#if defined (ARRAY_VARS)
  /* If parser_state includes PST_COMPASSIGN, FLAGS will include 1, so we are
     parsing the contents of a compound assignment. If parser_state includes
     PST_REPARSE, we are in the middle of an assignment statement and breaking
     the words between the parens into words and assignment statements, but
     we don't need to check for that right now. Within a compound assignment,
     the subscript is required to make the word an assignment statement. If
     we don't have a subscript, even if the word is a valid assignment
     statement otherwise, we don't want to treat it as one. */
  if ((flags & 1) && c != '[')		/* ] */
    return (0);
  else if ((flags & 1) == 0 && legal_variable_starter (c) == 0)
#else
  if (legal_variable_starter (c) == 0)
#endif
    return (0);

  while (c = string[indx])
    {
      /* The following is safe.  Note that '=' at the start of a word
	 is not an assignment statement. */
      if (c == '=')
	return (indx);

#if defined (ARRAY_VARS)
      if (c == '[')
	{
	  newi = skipsubscript (string, indx, (flags & 2) ? 1 : 0);
	  /* XXX - why not check for blank subscripts here, if we do in
	     valid_array_reference? */
	  if (string[newi++] != ']')
	    return (0);
	  if (string[newi] == '+' && string[newi+1] == '=')
	    return (newi + 1);
	  return ((string[newi] == '=') ? newi : 0);
	}
#endif /* ARRAY_VARS */

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

char **add_export_env(char *envstr)
{
	if (export_env_index >= (export_env_size - 1))
	{
		export_env_size += 16;
		export_env = strvec_resize(export_env, export_env_size);
		environ = export_env;
	}
	export_env[export_env_index++] = savestring (envstr);
	export_env[export_env_index] = NULL;
}

char	**add_or_supercede_exported_var (char *assign, int do_alloc)
{
	int i;
	int equal_offset;

	equal_offset = assignment(assign, 0);
	if (!equal_offset)
		return (export_env);
	if (assign[equal_offset + 1] == '(' && ft_strncmp(assign + equal_offset + 2, ") {", 3) == 0)
		equal_offset += 4;
	for (i = 0; i < export_env_index; i++)
	{
		if (ft_strncmp(assign, export_env[i], equal_offset + 1) == 0)
		{
			export_env[i] = ft_strdup(assign);
			return (export_env);
		}
	}
	add_to_export_env(assign);
	return (export_env);
}

void	update_export_env_inplace(char *env_prefix, int preflen, char *value)
{
	char *evar;

	evar = ft_malloc (ft_strlen(value) + preflen + 1);
	strcpy (evar, env_prefix);
	if (value)
		strcpy (evar + preflen, value);
	add_or_supercede_exported_var(evar, 0);
}
