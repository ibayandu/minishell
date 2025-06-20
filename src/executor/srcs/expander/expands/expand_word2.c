/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 05:51:55 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/19 21:31:10 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include "expander.h"

t_word_list	*expand_word_internal(t_word *word, int quoted, int isexp, int *expanded_something)
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
	int				quoted_state;
	int				has_quoted_ifs;
	int				split_on_spaces;
	int				local_expanded;
	int				tflag;
	int				pflags;
	int				assignoff;
	unsigned char	c;
	int				t_index;
	char			twochars[2];


	istring_size = DEFAULT_INITIAL_ARRAY_SIZE;
	istring = ft_malloc(istring_size);
	istring_index = 0;
	istring[istring_index] = '\0';
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
	for (sindex = 0; ;)
	{
		c = string[sindex];

		/* Case on top-level character. */
		switch (c)
		{
		case '\0':
		goto finished_with_string;

		case CTLESC:
		sindex++;
	#if HANDLE_MULTIBYTE
		if (mb_cur_max > 1 && string[sindex])
			{
			SADD_MBQCHAR_BODY(temp, string, sindex, string_size);
			}
		else
	#endif
			{
			temp = (char *)xmalloc (3);
			temp[0] = CTLESC;
			temp[1] = c = string[sindex];
			temp[2] = '\0';
			}

	dollar_add_string:
		if (string[sindex])
			sindex++;

	add_string:
		if (temp)
			{
			istring = sub_append_string (temp, istring, &istring_index, &istring_size);
			temp = (char *)0;
			}

		break;

	#if defined (PROCESS_SUBSTITUTION)
		/* Process substitution. */
		case '<':
		case '>':
		{
			/* XXX - technically this should only be expanded at the start
			of a word */
			if (string[++sindex] != LPAREN || (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) || (word->flags & W_NOPROCSUB))
			{
			sindex--;	/* add_character: label increments sindex */
			goto add_character;
			}
			else
			t_index = sindex + 1; /* skip past both '<' and LPAREN */

			temp1 = extract_process_subst (string, (c == '<') ? "<(" : ">(", &t_index, 0); /*))*/
			sindex = t_index;

			/* If the process substitution specification is `<()', we want to
			open the pipe for writing in the child and produce output; if
			it is `>()', we want to open the pipe for reading in the child
			and consume input. */
			temp = temp1 ? process_substitute (temp1, (c == '>')) : (char *)0;

			FREE (temp1);

			goto dollar_add_string;
		}
	#endif /* PROCESS_SUBSTITUTION */

	#if defined (ARRAY_VARS)
		case '[':		/*]*/
		if ((quoted & Q_ARITH) == 0 || shell_compatibility_level <= 51)
			{
			if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c) && (quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)) == 0)
			goto add_ifs_character;
			else
			goto add_character;
			}
		else
			{
			temp = expand_array_subscript (string, &sindex, quoted, word->flags);
			goto add_string;
			}
	#endif

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
		if ((word->flags & W_ASSIGNMENT) &&
			(posixly_correct == 0 || (word->flags & W_TILDEEXP)) &&
			assignoff == -1 && sindex > 0)
			assignoff = sindex;
		if (sindex == assignoff && string[sindex+1] == '~')	/* XXX */
			internal_tilde = 1;

		if (word->flags & W_ASSIGNARG)
			word->flags |= W_ASSIGNRHS;		/* affects $@ */

		if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
			{
			has_quoted_ifs++;
			goto add_ifs_character;
			}
		else
			goto add_character;

		case ':':
		if (word->flags & (W_NOTILDE|W_NOASSNTILDE))
			{
			if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
			goto add_ifs_character;
			else
			goto add_character;
			}

		if ((word->flags & (W_ASSIGNMENT|W_ASSIGNRHS)) &&
			(posixly_correct == 0 || (word->flags & W_TILDEEXP)) &&
			string[sindex+1] == '~')
			internal_tilde = 1;

		if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
			goto add_ifs_character;
		else
			goto add_character;

		case '~':
		/* If the word isn't supposed to be tilde expanded, or we're not
			at the start of a word or after an unquoted : or = in an
			assignment statement, we don't do tilde expansion.  We don't
			do tilde expansion if quoted or in an arithmetic context. */

		if ((word->flags & W_NOTILDE) ||
			(sindex > 0 && (internal_tilde == 0)) ||
			(quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)))
			{
			internal_tilde = 0;
			if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c) && (quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)) == 0)
			goto add_ifs_character;
			else
			goto add_character;
			}

		if (word->flags & W_ASSIGNRHS)
			tflag = 2;
		else if (word->flags & (W_ASSIGNMENT|W_TILDEEXP))
			tflag = 1;
		else
			tflag = 0;

		temp = bash_tilde_find_word (string + sindex, tflag, &t_index);

		internal_tilde = 0;

		if (temp && *temp && t_index > 0)
			{
			temp1 = bash_tilde_expand (temp, tflag);
			if  (temp1 && *temp1 == '~' && STREQ (temp, temp1))
			{
			FREE (temp);
			FREE (temp1);
			goto add_character;		/* tilde expansion failed */
			}
			free (temp);
			temp = temp1;
			sindex += t_index;
			goto add_quoted_string;		/* XXX was add_string */
			}
		else
			{
			FREE (temp);
			goto add_character;
			}

		case '$':
		if (expanded_something)
			*expanded_something = 1;
		local_expanded = 1;

		temp_has_dollar_at = 0;
		pflags = (word->flags & W_NOCOMSUB) ? PF_NOCOMSUB : 0;
		if (word->flags & W_NOSPLIT2)
			pflags |= PF_NOSPLIT2;
		if (word->flags & W_ASSIGNRHS)
			pflags |= PF_ASSIGNRHS;
		if (word->flags & W_COMPLETE)
			pflags |= PF_COMPLETE;

		tword = param_expand (string, &sindex, quoted, expanded_something,
					&temp_has_dollar_at, &quoted_dollar_at,
					&had_quoted_null, pflags);
		has_dollar_at += temp_has_dollar_at;
		split_on_spaces += (tword->flags & W_SPLITSPACE);

		if (tword == &expand_wdesc_error || tword == &expand_wdesc_fatal)
			{
			free (string);
			free (istring);
			return ((tword == &expand_wdesc_error) ? &expand_word_error
								: &expand_word_fatal);
			}
		if (contains_dollar_at && has_dollar_at)
			*contains_dollar_at = 1;

		if (tword && (tword->flags & W_HASQUOTEDNULL))
			had_quoted_null = 1;		/* note for later */
		if (tword && (tword->flags & W_SAWQUOTEDNULL))
			had_quoted_null = 1;		/* XXX */

		temp = tword ? tword->word : (char *)NULL;
		dispose_word_desc (tword);

		/* Kill quoted nulls; we will add them back at the end of
			expand_word_internal if nothing else in the string */
		if (had_quoted_null && temp && QUOTED_NULL (temp))
			{
			FREE (temp);
			temp = (char *)NULL;
			}

		goto add_string;
		break;

		case '`':		/* Backquoted command substitution. */
		{
			t_index = sindex++;

			temp = string_extract (string, &sindex, "`", (word->flags & W_COMPLETE) ? SX_COMPLETE : SX_REQMATCH);
			/* The test of sindex against t_index is to allow bare instances of
			` to pass through, for backwards compatibility. */
			if (temp == &extract_string_error || temp == &extract_string_fatal)
			{
			if (sindex - 1 == t_index)
			{
				sindex = t_index;
				goto add_character;
			}
			set_exit_status (EXECUTION_FAILURE);
			report_error (_("bad substitution: no closing \"`\" in %s") , string+t_index);
			free (string);
			free (istring);
			return ((temp == &extract_string_error) ? &expand_word_error
								: &expand_word_fatal);
			}

			if (expanded_something)
			*expanded_something = 1;
			local_expanded = 1;

			if (word->flags & W_NOCOMSUB)
			/* sindex + 1 because string[sindex] == '`' */
			temp1 = substring (string, t_index, sindex + 1);
			else
			{
			de_backslash (temp);
			tword = command_substitute (temp, quoted, PF_BACKQUOTE);
			temp1 = tword ? tword->word : (char *)NULL;
			if (tword)
			dispose_word_desc (tword);
			}
			FREE (temp);
			temp = temp1;
			goto dollar_add_string;
		}

		case '\\':
		if (string[sindex + 1] == '\n')
			{
			sindex += 2;
			continue;
			}

		c = string[++sindex];

		/* "However, the double-quote character ( '"' ) shall not be treated
			specially within a here-document, except when the double-quote
			appears within "$()", "``", or "${}"." */
		if ((quoted & Q_HERE_DOCUMENT) && (quoted & Q_DOLBRACE) && c == '"')
			tflag = CBSDQUOTE;		/* special case */
		else if (quoted & Q_HERE_DOCUMENT)
			tflag = CBSHDOC;
		else if (quoted & Q_DOUBLE_QUOTES)
			tflag = CBSDQUOTE;
		else
			tflag = 0;

		/* From Posix discussion on austin-group list:  Backslash escaping
			a } in ${...} is removed.  Issue 0000221 */
		if ((quoted & Q_DOLBRACE) && c == RBRACE)
			{
			SCOPY_CHAR_I (twochars, CTLESC, c, string, sindex, string_size);
			}
		/* This is the fix for " $@\ " */
		else if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && ((sh_syntaxtab[c] & tflag) == 0) && isexp == 0 && isifs (c))
			{
			RESIZE_MALLOCED_BUFFER (istring, istring_index, 2, istring_size,
						DEFAULT_ARRAY_SIZE);
			istring[istring_index++] = CTLESC;
			istring[istring_index++] = '\\';
			istring[istring_index] = '\0';

			SCOPY_CHAR_I (twochars, CTLESC, c, string, sindex, string_size);
			}
		else if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && c == 0)
			{
			RESIZE_MALLOCED_BUFFER (istring, istring_index, 2, istring_size,
						DEFAULT_ARRAY_SIZE);
			istring[istring_index++] = CTLESC;
			istring[istring_index++] = '\\';
			istring[istring_index] = '\0';
			break;
			}
		else if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && ((sh_syntaxtab[c] & tflag) == 0))
			{
			SCOPY_CHAR_I (twochars, '\\', c, string, sindex, string_size);
			}
		else if (c == 0)
			{
			c = CTLNUL;
			sindex--;		/* add_character: label increments sindex */
			goto add_character;
			}
		else
			{
			SCOPY_CHAR_I (twochars, CTLESC, c, string, sindex, string_size);
			}

		sindex++;
	add_twochars:
		/* BEFORE jumping here, we need to increment sindex if appropriate */
		RESIZE_MALLOCED_BUFFER (istring, istring_index, 2, istring_size,
					DEFAULT_ARRAY_SIZE);
		istring[istring_index++] = twochars[0];
		istring[istring_index++] = twochars[1];
		istring[istring_index] = '\0';

		break;

		case '"':
		/* XXX - revisit this */
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

			temp = (char *)NULL;

			temp_has_dollar_at = 0;	/* does this quoted (sub)string include $@? */
			/* Need to get W_HASQUOTEDNULL flag through this function. */
			/* XXX - preserve Q_ARITH here? */
			list = expand_word_internal (tword, Q_DOUBLE_QUOTES|(quoted&Q_ARITH), 0, &temp_has_dollar_at, (int *)NULL);
			has_dollar_at += temp_has_dollar_at;

			if (list == &expand_word_error || list == &expand_word_fatal)
			{
			free (istring);
			free (string);
			/* expand_word_internal has already freed temp_word->word
				for us because of the way it prints error messages. */
			tword->word = (char *)NULL;
			dispose_word (tword);
			return list;
			}

			dispose_word (tword);

			/* "$@" (a double-quoted dollar-at) expands into nothing,
			not even a NULL word, when there are no positional
			parameters.  Posix interp 888 says that other parts of the
			word that expand to quoted nulls result in quoted nulls, so
			we can't just throw the entire word away if we have "$@"
			anywhere in it.  We use had_quoted_null to keep track */
			if (list == 0 && temp_has_dollar_at)	/* XXX - was has_dollar_at */
			{
			quoted_dollar_at++;
			break;
			}

			/* If this list comes back with a quoted null from expansion,
			we have either "$x" or "$@" with $1 == ''. In either case,
			we need to make sure we add a quoted null argument and
			disable the special handling that "$@" gets. */
			if (list && list->word && list->next == 0 && (list->word->flags & W_HASQUOTEDNULL))
			{
			if (had_quoted_null && temp_has_dollar_at)
				quoted_dollar_at++;
			had_quoted_null = 1;		/* XXX */
			}

			/* If we get "$@", we know we have expanded something, so we
			need to remember it for the final split on $IFS.  This is
			a special case; it's the only case where a quoted string
			can expand into more than one word.  It's going to come back
			from the above call to expand_word_internal as a list with
			multiple words. */
			if (list)
			dequote_list (list);

			if (temp_has_dollar_at)		/* XXX - was has_dollar_at */
			{
			quoted_dollar_at++;
			if (contains_dollar_at)
				*contains_dollar_at = 1;
			if (expanded_something)
				*expanded_something = 1;
			local_expanded = 1;
			}
			}
		else
			{
			/* What we have is "".  This is a minor optimization. */
			FREE (temp);
			list = (WORD_LIST *)NULL;
			had_quoted_null = 1;	/* note for later */
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
			/* Testing quoted_dollar_at makes sure that "$@" is
				split correctly when $IFS does not contain a space. */
			temp = quoted_dollar_at
					? string_list_dollar_at (list, Q_DOUBLE_QUOTES, 0)
					: string_list (quote_list (list));
			dispose_words (list);
			goto add_string;
			}
			else
			{
			temp = savestring (list->word->word);
			tflag = list->word->flags;
			dispose_words (list);

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
			temp = (char *)NULL;

		if (temp == 0 && quoted_state == PARTIALLY_QUOTED)
			had_quoted_null = 1;	/* note for later */

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
			continue;

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

		/* break; */

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
			{
			free (temp);
			temp = (char *)NULL;
			}
		else
			remove_quoted_escapes (temp);	/* ??? */

		if (temp == 0 && quoted_state == PARTIALLY_QUOTED)
			had_quoted_null = 1;	/* note for later */

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

		/* break; */

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
	#if HANDLE_MULTIBYTE
			/* XXX - should make sure that c is actually multibyte,
				otherwise we can use the twochars branch */
			if (mb_cur_max > 1)
				sindex--;

			if (mb_cur_max > 1)
				{
				SADD_MBQCHAR_BODY(temp, string, sindex, string_size);
				}
			else
	#endif
				{
				twochars[0] = CTLESC;
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

		/* Next character. */
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
	#if 0
		if (quoted_dollar_at == 0 && (had_quoted_null || quoted_state == PARTIALLY_QUOTED))
	#else
		if (had_quoted_null || (quoted_dollar_at == 0 && quoted_state == PARTIALLY_QUOTED))
	#endif
		{
		istring[0] = CTLNUL;
		istring[1] = '\0';
		tword = alloc_word_desc ();
		tword->word = istring;
		istring = 0;		/* avoid later free() */
		tword->flags |= W_HASQUOTEDNULL;		/* XXX */
		list = make_word_list (tword, (WORD_LIST *)NULL);
		if (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES))
			tword->flags |= W_QUOTED;
		}
		/* According to sh, ksh, and Posix.2, if a word expands into nothing
		and a double-quoted "$@" appears anywhere in it, then the entire
		word is removed. */
		/* XXX - exception appears to be that quoted null strings result in
		null arguments */
		else  if (quoted_state == UNQUOTED || quoted_dollar_at)
		list = (WORD_LIST *)NULL;
		else
		list = (WORD_LIST *)NULL;
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
		list = make_word_list (tword, (WORD_LIST *)NULL);
		}
	else if (word->flags & W_ASSIGNRHS)
		{
		list = list_string (istring, "", quoted);
		tword = list->word;
		if (had_quoted_null && QUOTED_NULL (istring))
		tword->flags |= W_HASQUOTEDNULL;
		free (list);
		free (istring);
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
			list = list_string (istring, " \t\n", 1);	/* XXX quoted == 1? */
		else
			list = list_string (istring, " ", 1);	/* XXX quoted == 1? */
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
		if (tword->word != istring)
			free (istring);
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
		if (tword->word != istring)
			free (istring);
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
		list = make_word_list (tword, (WORD_LIST *)NULL);
		}
		}

	free (istring);
	return (list);
}
