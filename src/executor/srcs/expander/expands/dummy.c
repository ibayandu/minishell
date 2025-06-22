// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   expand_word.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/06/20 15:01:10 by yzeybek           #+#    #+#             */
// /*   Updated: 2025/06/21 17:50:02 by yzeybek          ###   ########.tr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "makers.h"
// #include "libft.h"
// #include "expander.h"

// t_word_list	*expand_word_internal(t_word *word, int quoted, int isexp, int *contains_dollar_at, int *expanded_something)
// {
// 	t_word_list		*list;
// 	t_word			*tword;
// 	char			*istring;
// 	size_t			istring_size;
// 	size_t			istring_index;
// 	char			*temp;
// 	char			*temp1;
// 	char			*string;
// 	size_t			string_size;
// 	int				sindex;
// 	int				quoted_dollar_at;
// 	int				quoted_state;
// 	int				had_quoted_null;
// 	int				has_quoted_ifs;
// 	int				has_dollar_at;
// 	int				temp_has_dollar_at;
// 	int				internal_tilde;
// 	int				split_on_spaces;
// 	int				local_expanded;
// 	int				tflag;
// 	int				pflags;
// 	int				assignoff;
// 	unsigned char	c;
// 	int				t_index;
// 	char			twochars[2];
// 	DECLARE_MBSTATE;

// 	if (STREQ (word->word, "\"$@\"") && (word->flags == (W_HASDOLLAR|W_QUOTED)) && dollar_vars[1])
// 	{
// 		if (contains_dollar_at)
// 			*contains_dollar_at = 1;
// 		if (expanded_something)
// 			*expanded_something = 1;
// 		if (cached_quoted_dollar_at)
// 			return (copy_word_list (cached_quoted_dollar_at));
// 		list = list_rest_of_args ();
// 		list = quote_list (list);
// 		cached_quoted_dollar_at = copy_word_list (list);
// 		return (list);
// 	}
// 	istring_size = DEFAULT_INITIAL_ARRAY_SIZE;
// 	istring = (char *)xmalloc(istring_size);
// 	istring_index = 0;
// 	istring[istring_index] = '\0';
// 	quoted_dollar_at = 0;
// 	had_quoted_null = 0;
// 	has_dollar_at = 0;
// 	has_quoted_ifs = 0;
// 	split_on_spaces = 0;
// 	internal_tilde = 0;
// 	quoted_state = UNQUOTED;
// 	string = word->word;
// 	if (string == 0)
// 		goto finished_with_string;

// 	if (MB_CUR_MAX > 1)
// 		string_size = strlen(string);
// 	else
// 		string_size = 1;
// 	if (contains_dollar_at)
// 		*contains_dollar_at = 0;
// 	assignoff = -1;
// 	sindex = 0;
// 	while (1)
// 	{
// 		c = string[sindex];
// 		switch (c)
// 		{
// 			case '\0':
// 				goto finished_with_string;
// 			case CTLESC:
// 				sindex++;
// 				#if HANDLE_MULTIBYTE
// 					if (mb_cur_max > 1 && string[sindex])
// 						{
// 						SADD_MBQCHAR_BODY(temp, string, sindex, string_size);
// 						}
// 					else
// 				#endif
// 						{
// 						temp = (char *)xmalloc (3);
// 						temp[0] = CTLESC;
// 						temp[1] = c = string[sindex];
// 						temp[2] = '\0';
// 						}

// 				dollar_add_string:
// 					if (string[sindex])
// 						sindex++;

// 				add_string:
// 					if (temp)
// 						{
// 						istring = sub_append_string (temp, istring, &istring_index, &istring_size);
// 						temp = (char *)0;
// 						}

// 					break;

// 				#if defined (PROCESS_SUBSTITUTION)
// 					case '<':
// 					case '>':
// 					{
// 						if (string[++sindex] != LPAREN || (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) || (word->flags & W_NOPROCSUB))
// 						{
// 						sindex--;
// 						goto add_character;
// 						}
// 						else
// 						t_index = sindex + 1;
// 						temp1 = extract_process_subst (string, (c == '<') ? "<(" : ">(", &t_index, 0);
// 						sindex = t_index;
// 						temp = temp1 ? process_substitute (temp1, (c == '>')) : (char *)0;
// 						FREE (temp1);
// 						goto dollar_add_string;
// 					}
// 				#endif

// 				#if defined (ARRAY_VARS)
// 					case '[':
// 					if ((quoted & Q_ARITH) == 0 || shell_compatibility_level <= 51)
// 						{
// 						if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c) && (quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)) == 0)
// 						goto add_ifs_character;
// 						else
// 						goto add_character;
// 						}
// 					else
// 						{
// 						temp = expand_array_subscript (string, &sindex, quoted, word->flags);
// 						goto add_string;
// 						}
// 				#endif

// 			case '=':
// 				if (word->flags & (W_ASSIGNRHS|W_NOTILDE))
// 				{
// 					if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
// 						goto add_ifs_character;
// 					else
// 						goto add_character;
// 				}
// 				if ((word->flags & W_ASSIGNMENT) && (posixly_correct == 0 || (word->flags & W_TILDEEXP)) && assignoff == -1 && sindex > 0)
// 					assignoff = sindex;
// 				if (sindex == assignoff && string[sindex+1] == '~')
// 					internal_tilde = 1;
// 				if (word->flags & W_ASSIGNARG)
// 					word->flags |= W_ASSIGNRHS;
// 				if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
// 				{
// 					has_quoted_ifs++;
// 					goto add_ifs_character;
// 				}
// 				else
// 					goto add_character;
// 			case ':':
// 				if (word->flags & (W_NOTILDE|W_NOASSNTILDE))
// 				{
// 					if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
// 						goto add_ifs_character;
// 					else
// 						goto add_character;
// 				}
// 				if ((word->flags & (W_ASSIGNMENT|W_ASSIGNRHS)) && (posixly_correct == 0 || (word->flags & W_TILDEEXP)) && string[sindex+1] == '~')
// 					internal_tilde = 1;
// 				if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
// 					goto add_ifs_character;
// 				else
// 					goto add_character;

// 			case '~':
// 				if ((word->flags & W_NOTILDE) || (sindex > 0 && (internal_tilde == 0)) || (quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)))
// 				{
// 					internal_tilde = 0;
// 					if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c) && (quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)) == 0)
// 						goto add_ifs_character;
// 					else
// 						goto add_character;
// 				}
// 				if (word->flags & W_ASSIGNRHS)
// 					tflag = 2;
// 				else if (word->flags & (W_ASSIGNMENT|W_TILDEEXP))
// 					tflag = 1;
// 				else
// 					tflag = 0;
// 				temp = bash_tilde_find_word (string + sindex, tflag, &t_index);
// 				internal_tilde = 0;
// 				if (temp && *temp && t_index > 0)
// 				{
// 					temp1 = bash_tilde_expand (temp, tflag);
// 					if  (temp1 && *temp1 == '~' && STREQ (temp, temp1))
// 					{
// 						FREE (temp);
// 						FREE (temp1);
// 						goto add_character;
// 					}
// 					free (temp);
// 					temp = temp1;
// 					sindex += t_index;
// 					goto add_quoted_string;
// 				}
// 				else
// 				{
// 					FREE (temp);
// 					goto add_character;
// 				}
// 			case '$':
// 				if (expanded_something)
// 					*expanded_something = 1;
// 				local_expanded = 1;

// 				temp_has_dollar_at = 0;
// 				pflags = (word->flags & W_NOCOMSUB) ? PF_NOCOMSUB : 0;
// 				if (word->flags & W_NOSPLIT2)
// 					pflags |= PF_NOSPLIT2;
// 				if (word->flags & W_ASSIGNRHS)
// 					pflags |= PF_ASSIGNRHS;
// 				if (word->flags & W_COMPLETE)
// 					pflags |= PF_COMPLETE;
// 				tword = param_expand (string, &sindex, quoted, expanded_something,
// 							&temp_has_dollar_at, &quoted_dollar_at,
// 							&had_quoted_null, pflags);
// 				has_dollar_at += temp_has_dollar_at;
// 				split_on_spaces += (tword->flags & W_SPLITSPACE);
// 				if (tword == &expand_wdesc_error || tword == &expand_wdesc_fatal)
// 				{
// 					free (string);
// 					free (istring);
// 					return ((tword == &expand_wdesc_error) ? &expand_word_error : &expand_word_fatal);
// 				}
// 				if (contains_dollar_at && has_dollar_at)
// 					*contains_dollar_at = 1;
// 				if (tword && (tword->flags & W_HASQUOTEDNULL))
// 					had_quoted_null = 1;
// 				if (tword && (tword->flags & W_SAWQUOTEDNULL))
// 					had_quoted_null = 1;
// 				temp = tword ? tword->word : (char *)NULL;
// 				dispose_word_desc (tword);
// 				if (had_quoted_null && temp && QUOTED_NULL (temp))
// 				{
// 					FREE (temp);
// 					temp = (char *)NULL;
// 				}
// 				goto add_string;
// 				break;
// 			case '`':
// 			{
// 				t_index = sindex++;
// 				temp = string_extract (string, &sindex, "`", (word->flags & W_COMPLETE) ? SX_COMPLETE : SX_REQMATCH);
// 				if (temp == &extract_string_error || temp == &extract_string_fatal)
// 				{
// 					if (sindex - 1 == t_index)
// 					{
// 						sindex = t_index;
// 						goto add_character;
// 					}
// 					set_exit_status (EXECUTION_FAILURE);
// 					report_error (_("bad substitution: no closing \"`\" in %s") , string+t_index);
// 					free (string);
// 					free (istring);
// 					return ((temp == &extract_string_error) ? &expand_word_error : &expand_word_fatal);
// 				}
// 				if (expanded_something)
// 				*expanded_something = 1;
// 				local_expanded = 1;
// 				if (word->flags & W_NOCOMSUB)
// 					temp1 = substring (string, t_index, sindex + 1);
// 				else
// 				{
// 					de_backslash (temp);
// 					tword = command_substitute (temp, quoted, PF_BACKQUOTE);
// 					temp1 = tword ? tword->word : (char *)NULL;
// 					if (tword)
// 						dispose_word_desc (tword);
// 				}
// 				FREE (temp);
// 				temp = temp1;
// 				goto dollar_add_string;
// 			}
// 			case '\\':
// 				if (string[sindex + 1] == '\n')
// 				{
// 					sindex += 2;
// 					continue;
// 				}
// 				c = string[++sindex];
// 				if ((quoted & Q_HERE_DOCUMENT) && (quoted & Q_DOLBRACE) && c == '"')
// 					tflag = CBSDQUOTE;		/* special case */
// 				else if (quoted & Q_HERE_DOCUMENT)
// 					tflag = CBSHDOC;
// 				else if (quoted & Q_DOUBLE_QUOTES)
// 					tflag = CBSDQUOTE;
// 				else
// 					tflag = 0;
// 				if ((quoted & Q_DOLBRACE) && c == RBRACE)
// 					SCOPY_CHAR_I (twochars, CTLESC, c, string, sindex, string_size);
// 				else if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && ((sh_syntaxtab[c] & tflag) == 0) && isexp == 0 && isifs (c))
// 				{
// 					RESIZE_MALLOCED_BUFFER (istring, istring_index, 2, istring_size, DEFAULT_ARRAY_SIZE);
// 					istring[istring_index++] = CTLESC;
// 					istring[istring_index++] = '\\';
// 					istring[istring_index] = '\0';
// 					SCOPY_CHAR_I (twochars, CTLESC, c, string, sindex, string_size);
// 				}
// 				else if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && c == 0)
// 				{
// 					RESIZE_MALLOCED_BUFFER (istring, istring_index, 2, istring_size, DEFAULT_ARRAY_SIZE);
// 					istring[istring_index++] = CTLESC;
// 					istring[istring_index++] = '\\';
// 					istring[istring_index] = '\0';
// 					break;
// 				}
// 				else if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && ((sh_syntaxtab[c] & tflag) == 0))
// 					SCOPY_CHAR_I (twochars, '\\', c, string, sindex, string_size);
// 				else if (c == 0)
// 				{
// 					c = CTLNUL;
// 					sindex--;
// 					goto add_character;
// 				}
// 				else
// 				{
// 					SCOPY_CHAR_I (twochars, CTLESC, c, string, sindex, string_size);
// 				}

// 				sindex++;
// 				add_twochars:
// 				RESIZE_MALLOCED_BUFFER (istring, istring_index, 2, istring_size, DEFAULT_ARRAY_SIZE);
// 				istring[istring_index++] = twochars[0];
// 				istring[istring_index++] = twochars[1];
// 				istring[istring_index] = '\0';
// 				break;
// 			case '"':

// 				if ((quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)) && ((quoted & Q_ARITH) == 0))
// 					goto add_character;
// 				t_index = ++sindex;
// 				temp = string_extract_double_quoted (string, &sindex, (word->flags & W_COMPLETE) ? SX_COMPLETE : 0);
// 				quoted_state = (t_index == 1 && string[sindex] == '\0') ? WHOLLY_QUOTED : PARTIALLY_QUOTED;
// 				if (temp && *temp)
// 				{
// 					tword = alloc_word_desc ();
// 					tword->word = temp;

// 					if (word->flags & W_ASSIGNARG)
// 						tword->flags |= word->flags & (W_ASSIGNARG|W_ASSIGNRHS); /* affects $@ */
// 					if (word->flags & W_COMPLETE)
// 						tword->flags |= W_COMPLETE;	/* for command substitutions */
// 					if (word->flags & W_NOCOMSUB)
// 						tword->flags |= W_NOCOMSUB;
// 					if (word->flags & W_NOPROCSUB)
// 						tword->flags |= W_NOPROCSUB;
// 					if (word->flags & W_ASSIGNRHS)
// 						tword->flags |= W_ASSIGNRHS;
// 					temp = (char *)NULL;
// 					temp_has_dollar_at = 0;
// 					list = expand_word_internal (tword, Q_DOUBLE_QUOTES|(quoted&Q_ARITH), 0, &temp_has_dollar_at, (int *)NULL);
// 					has_dollar_at += temp_has_dollar_at;
// 					if (list == &expand_word_error || list == &expand_word_fatal)
// 					{
// 						free (istring);
// 						free (string);
// 						tword->word = (char *)NULL;
// 						dispose_word (tword);
// 						return list;
// 					}
// 					dispose_word (tword);
// 					if (list == 0 && temp_has_dollar_at)
// 					{
// 						quoted_dollar_at++;
// 						break;
// 					}
// 					if (list && list->word && list->next == 0 && (list->word->flags & W_HASQUOTEDNULL))
// 					{
// 						if (had_quoted_null && temp_has_dollar_at) quoted_dollar_at++;
// 							had_quoted_null = 1;
// 					}
// 					if (list)
// 						dequote_list (list);
// 					if (temp_has_dollar_at)
// 					{
// 						quoted_dollar_at++;
// 						if (contains_dollar_at)
// 							*contains_dollar_at = 1;
// 						if (expanded_something)
// 							*expanded_something = 1;
// 						local_expanded = 1;
// 					}
// 				}
// 				else
// 				{
// 					FREE (temp);
// 					list = (WORD_LIST *)NULL;
// 					had_quoted_null = 1;
// 				}
// 				if (list)
// 				{
// 					if (list->next)
// 					{
// 						temp = quoted_dollar_at ? string_list_dollar_at (list, Q_DOUBLE_QUOTES, 0) : string_list (quote_list (list));
// 						dispose_words (list);
// 						goto add_string;
// 					}
// 					else
// 					{
// 						temp = savestring (list->word->word);
// 						tflag = list->word->flags;
// 						dispose_words (list);
// 						if ((tflag & W_HASQUOTEDNULL) && QUOTED_NULL (temp) == 0)
// 							remove_quoted_nulls (temp);
// 					}
// 				}
// 				else
// 					temp = (char *)NULL;
// 				if (temp == 0 && quoted_state == PARTIALLY_QUOTED)
// 					had_quoted_null = 1;
// 				if (temp == 0 && quoted_state == PARTIALLY_QUOTED && quoted == 0 && (word->flags & (W_NOSPLIT|W_EXPANDRHS|W_ASSIGNRHS)) == W_EXPANDRHS)
// 				{
// 					c = CTLNUL;
// 					sindex--;
// 					had_quoted_null = 1;
// 					goto add_character;
// 				}
// 				if (temp == 0 && quoted_state == PARTIALLY_QUOTED && (word->flags & (W_NOSPLIT|W_NOSPLIT2)))
// 					continue;
// 				add_quoted_string:
// 				if (temp)
// 				{
// 					temp1 = temp;
// 					temp = quote_string (temp);
// 					free (temp1);
// 					goto add_string;
// 				}
// 				else
// 				{

// 					c = CTLNUL;
// 					sindex--;
// 					had_quoted_null = 1;
// 					goto add_character;
// 				}
// 			case '\'':
// 				if ((quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)))
// 					goto add_character;
// 				t_index = ++sindex;
// 				temp = string_extract_single_quoted (string, &sindex, 0);
// 				quoted_state = (t_index == 1 && string[sindex] == '\0') ? WHOLLY_QUOTED : PARTIALLY_QUOTED;
// 				if (*temp == '\0')
// 				{
// 					free (temp);
// 					temp = (char *)NULL;
// 				}
// 				else
// 					remove_quoted_escapes (temp);
// 				if (temp == 0 && quoted_state == PARTIALLY_QUOTED)
// 					had_quoted_null = 1;
// 				if (temp == 0 && quoted_state == PARTIALLY_QUOTED && quoted == 0 && (word->flags & (W_NOSPLIT|W_EXPANDRHS|W_ASSIGNRHS)) == W_EXPANDRHS)
// 				{
// 					c = CTLNUL;
// 					sindex--;
// 					goto add_character;
// 				}
// 				if (temp == 0 && (quoted_state == PARTIALLY_QUOTED) && (word->flags & (W_NOSPLIT|W_NOSPLIT2)))
// 					continue;
// 				if (temp == 0)
// 				{
// 					c = CTLNUL;
// 					sindex--;
// 					goto add_character;
// 				}
// 				else
// 					goto add_quoted_string;
// 			case ' ':
// 				if (ifs_is_null || split_on_spaces || ((word->flags & (W_NOSPLIT|W_NOSPLIT2|W_ASSIGNRHS)) && (word->flags & W_EXPANDRHS) == 0))
// 				{
// 					if (string[sindex])
// 						sindex++;
// 					twochars[0] = CTLESC;
// 					twochars[1] = c;
// 					goto add_twochars;
// 				}
// 			default:
// 				add_ifs_character:
// 				if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) || (isexp == 0 && isifs (c) && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0))
// 				{
// 					if ((quoted&(Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) == 0)
// 						has_quoted_ifs++;
// 					add_quoted_character:
// 					if (string[sindex])	/* from old goto dollar_add_string */
// 						sindex++;
// 					if (c == 0)
// 					{
// 						c = CTLNUL;
// 						goto add_character;
// 					}
// 					else
// 					{
// 						#if HANDLE_MULTIBYTE
// 						if (mb_cur_max > 1)
// 							sindex--;
// 						if (mb_cur_max > 1)
// 							SADD_MBQCHAR_BODY(temp, string, sindex, string_size);
// 						else
// 						#endif
// 						{
// 							twochars[0] = CTLESC;
// 							twochars[1] = c;
// 							goto add_twochars;
// 						}
// 					}
// 				}
// 				SADD_MBCHAR (temp, string, sindex, string_size);
// 				add_character:
// 				RESIZE_MALLOCED_BUFFER (istring, istring_index, 1, istring_size, DEFAULT_ARRAY_SIZE);
// 				istring[istring_index++] = c;
// 				istring[istring_index] = '\0';
// 				sindex++;
// 		}
// 	}

// 	finished_with_string:
// 	if (*istring == '\0')
// 	{
// 		if (had_quoted_null || (quoted_dollar_at == 0 && quoted_state == PARTIALLY_QUOTED))
// 		{
// 			istring[0] = CTLNUL;
// 			istring[1] = '\0';
// 			tword = alloc_word_desc ();
// 			tword->word = istring;
// 			istring = 0;
// 			tword->flags |= W_HASQUOTEDNULL;
// 			list = make_word_list (tword, (WORD_LIST *)NULL);
// 			if (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES))
// 				tword->flags |= W_QUOTED;
// 		}
// 		else  if (quoted_state == UNQUOTED || quoted_dollar_at)
// 			list = (WORD_LIST *)NULL;
// 		else
// 			list = (WORD_LIST *)NULL;
// 	}
// 	else if (word->flags & W_NOSPLIT)
// 	{
// 		tword = alloc_word_desc ();
// 		tword->word = istring;
// 		if (had_quoted_null && QUOTED_NULL (istring))
// 		tword->flags |= W_HASQUOTEDNULL;
// 		istring = 0;
// 		if (word->flags & W_ASSIGNMENT)
// 		tword->flags |= W_ASSIGNMENT;
// 		if (word->flags & W_COMPASSIGN)
// 		tword->flags |= W_COMPASSIGN;	/* XXX */
// 		if (word->flags & W_NOGLOB)
// 		tword->flags |= W_NOGLOB;	/* XXX */
// 		if (word->flags & W_NOBRACE)
// 		tword->flags |= W_NOBRACE;
// 		if (word->flags & W_ARRAYREF)
// 		tword->flags |= W_ARRAYREF;
// 		if (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES))
// 		tword->flags |= W_QUOTED;
// 		list = make_word_list (tword, (WORD_LIST *)NULL);
// 	}
// 	else if (word->flags & W_ASSIGNRHS)
// 	{
// 		list = list_string (istring, "", quoted);
// 		tword = list->word;
// 		if (had_quoted_null && QUOTED_NULL (istring))
// 		tword->flags |= W_HASQUOTEDNULL;
// 		free (list);
// 		free (istring);
// 		istring = 0;
// 		goto set_word_flags;
// 	}
// 	else
// 	{
// 		char *ifs_chars;
// 		ifs_chars = (quoted_dollar_at || has_dollar_at) ? ifs_value : (char *)NULL;
// 		if (split_on_spaces)
// 		{
// 			if (ifs_is_set == 0)
// 				list = list_string (istring, " \t\n", 1);
// 			else
// 				list = list_string (istring, " ", 1);
// 		}
// 		else if (has_dollar_at && quoted_dollar_at == 0 && ifs_chars && quoted == 0 && (word->flags & W_NOSPLIT2))
// 		{
// 			tword = alloc_word_desc ();
// 			if (*ifs_chars && *ifs_chars != ' ')
// 			{
// 				list = list_string (istring, *ifs_chars ? ifs_chars : " ", 1);
// 				tword->word = string_list (list);
// 			}
// 			else
// 				tword->word = istring;
// 			if (had_quoted_null && QUOTED_NULL (istring))
// 				tword->flags |= W_HASQUOTEDNULL;
// 			if (tword->word != istring)
// 				free (istring);
// 			istring = 0;
// 			goto set_word_flags;
// 		}
// 		else if (has_dollar_at && ifs_chars)
// 			list = list_string (istring, *ifs_chars ? ifs_chars : " ", 1);
// 		else
// 		{
// 			tword = alloc_word_desc ();
// 			if (expanded_something && *expanded_something == 0 && has_quoted_ifs)
// 				tword->word = remove_quoted_ifs (istring);
// 			else
// 				tword->word = istring;
// 			if (had_quoted_null && QUOTED_NULL (istring))
// 				tword->flags |= W_HASQUOTEDNULL;
// 			else if (had_quoted_null)
// 				tword->flags |= W_SAWQUOTEDNULL;
// 			if (tword->word != istring)
// 				free (istring);
// 			istring = 0;
// 			set_word_flags:
// 			if ((quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)) || (quoted_state == WHOLLY_QUOTED))
// 				tword->flags |= W_QUOTED;
// 			if (word->flags & W_ASSIGNMENT)
// 				tword->flags |= W_ASSIGNMENT;
// 			if (word->flags & W_COMPASSIGN)
// 				tword->flags |= W_COMPASSIGN;
// 			if (word->flags & W_NOGLOB)
// 				tword->flags |= W_NOGLOB;
// 			if (word->flags & W_NOBRACE)
// 				tword->flags |= W_NOBRACE;
// 			if (word->flags & W_ARRAYREF)
// 				tword->flags |= W_ARRAYREF;
// 			list = make_word_list (tword, (WORD_LIST *)NULL);
// 		}
// 	}
// 	free (istring);
// 	return (list);
// }

// WORD_LIST	*list_string(char *string, char *separators, int quoted)
// {
// 	WORD_LIST	*result;
// 	WORD_DESC	*t;
// 	char		*current_word;
// 	char		*s;
// 	int			sindex;
// 	int			sh_style_split;
// 	int			whitesep;
// 	int			xflags;
// 	int			free_word;
// 	size_t		slen;

// 	if (!string || !*string)
// 		return (NULL);
// 	sh_style_split = separators && separators[0] == ' ' &&
// 					separators[1] == '\t' &&
// 					separators[2] == '\n' &&
// 					separators[3] == '\0';
// 	for (xflags = 0, s = ifs_value; s && *s; s++)
// 	{
// 		if (*s == CTLESC) xflags |= SX_NOCTLESC;
// 		else if (*s == CTLNUL) xflags |= SX_NOESCCTLNUL;
// 	}
// 	slen = 0;
// 	if (!quoted && separators && *separators)
// 	{
// 		for (s = string; *s && issep (*s) && ifs_whitespace (*s); s++);
// 		if (!*s)
// 			return ((WORD_LIST *)NULL);
// 		string = s;
// 	}
// 	slen = STRLEN (string);
// 	for (result = (WORD_LIST *)NULL, sindex = 0; string[sindex]; )
// 	{
// 		current_word = string_extract_verbatim (string, slen, &sindex, separators, xflags);
// 		if (current_word == 0)
// 			break;
// 		free_word = 1;
// 		if (QUOTED_NULL (current_word))
// 		{
// 			t = alloc_word_desc ();
// 			t->word = make_quoted_char ('\0');
// 			t->flags |= W_QUOTED|W_HASQUOTEDNULL;
// 			result = make_word_list (t, result);
// 		}
// 		else if (current_word[0] != '\0')
// 		{
// 			remove_quoted_nulls (current_word);
// 			t = alloc_word_desc ();
// 			t->word = current_word;
// 			result = make_word_list (t, result);
// 			free_word = 0;
// 			result->word->flags &= ~W_HASQUOTEDNULL;
// 			if (quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT))
// 				result->word->flags |= W_QUOTED;
// 			if (current_word == 0 || current_word[0] == '\0')
// 				result->word->flags |= W_SAWQUOTEDNULL;
// 		}
// 		else if (!sh_style_split && !ifs_whitespace (string[sindex]))
// 		{
// 			t = alloc_word_desc ();
// 			t->word = make_quoted_char ('\0');
// 			t->flags |= W_QUOTED|W_HASQUOTEDNULL;
// 			result = make_word_list (t, result);
// 		}
// 		if (free_word)
// 			free (current_word);
// 		whitesep = string[sindex] && ifs_whitesep (string[sindex]);
// 		if (string[sindex])
// 		{
// 			DECLARE_MBSTATE;
// 			ADVANCE_CHAR (string, slen, sindex);
// 		}
// 		while (string[sindex] && ifs_whitesep (string[sindex]) && issep (string[sindex]))
// 			sindex++;
// 		if (string[sindex] && whitesep && issep (string[sindex]) && !ifs_whitesep (string[sindex]))
// 		{
// 			sindex++;
// 			while (string[sindex] && ifs_whitesep (string[sindex]) && isifs (string[sindex]))
// 				sindex++;
// 		}
// 	}
// 	return (REVERSE_LIST (result, WORD_LIST *));
// }

// WORD_DESC *param_expand(char *string, int *sindex, int quoted, int *expanded_something, int *contains_dollar_at, int *quoted_dollar_at_p, int *had_quoted_null_p, int pflags)
// {
// 	 char *temp, *temp1, uerror[3], *savecmd;
// 	 int zindex, t_index, expok, eflag;
// 	 unsigned char c;
// 	 intmax_t number;
// 	 SHELL_VAR *var;
// 	 WORD_LIST *list, *l;
// 	 WORD_DESC *tdesc, *ret;
// 	 int tflag, nullarg;

//    /*itrace("param_expand: `%s' pflags = %d", string+*sindex, pflags);*/
// 	 zindex = *sindex;
// 	 c = string[++zindex];

// 	 temp = (char *)NULL;
// 	 ret = tdesc = (WORD_DESC *)NULL;
// 	 tflag = 0;

// 	 /* Do simple cases first. Switch on what follows '$'. */
// 	 switch (c)
// 	   {
// 	   /* $0 .. $9? */
// 	   case '0':
// 	   case '1':
// 	   case '2':
// 	   case '3':
// 	   case '4':
// 	   case '5':
// 	   case '6':
// 	   case '7':
// 	   case '8':
// 	   case '9':
// 		 temp1 = dollar_vars[TODIGIT (c)];
// 		 /* This doesn't get called when (pflags&PF_IGNUNBOUND) != 0 */
// 		 if (unbound_vars_is_error && temp1 == (char *)NULL)
// 	   {
// 		 uerror[0] = '$';
// 		 uerror[1] = c;
// 		 uerror[2] = '\0';
// 		 set_exit_status (EXECUTION_FAILURE);
// 		 err_unboundvar (uerror);
// 		 return (interactive_shell ? &expand_wdesc_error : &expand_wdesc_fatal);
// 	   }
// 		 if (temp1)
// 	   temp = (*temp1 && (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)))
// 			 ? quote_string (temp1)
// 			 : quote_escapes (temp1);
// 		 else
// 	   temp = (char *)NULL;

// 		 break;

// 	   /* $$ -- pid of the invoking shell. */
// 	   case '$':
// 		 temp = itos (dollar_dollar_pid);
// 		 break;

// 	   /* $# -- number of positional parameters. */
// 	   case '#':
// 		 temp = itos (number_of_args ());
// 		 break;

// 	   /* $? -- return value of the last synchronous command. */
// 	   case '?':
// 		 temp = itos (last_command_exit_value);
// 		 break;

// 	   /* $- -- flags supplied to the shell on invocation or by `set'. */
// 	   case '-':
// 		 temp = which_set_flags ();
// 		 break;

// 		 /* $! -- Pid of the last asynchronous command. */
// 	   case '!':
// 		 /* If no asynchronous pids have been created, expand to nothing.
// 		If `set -u' has been executed, and no async processes have
// 		been created, this is an expansion error. */
// 		 if (last_asynchronous_pid == NO_PID)
// 	   {
// 		 if (expanded_something)
// 		   *expanded_something = 0;
// 		 temp = (char *)NULL;
// 		 if (unbound_vars_is_error && (pflags & PF_IGNUNBOUND) == 0)
// 		   {
// 			 uerror[0] = '$';
// 			 uerror[1] = c;
// 			 uerror[2] = '\0';
// 			 set_exit_status (EXECUTION_FAILURE);
// 			 err_unboundvar (uerror);
// 			 return (interactive_shell ? &expand_wdesc_error : &expand_wdesc_fatal);
// 		   }
// 	   }
// 		 else
// 	   temp = itos (last_asynchronous_pid);
// 		 break;

// 	   /* The only difference between this and $@ is when the arg is quoted. */
// 	   case '*':		/* `$*' */
// 		 list = list_rest_of_args ();

// 		 /* If there are no command-line arguments, this should just
// 		disappear if there are other characters in the expansion,
// 		even if it's quoted. */
// 		 if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && list == 0)
// 	   temp = (char *)NULL;
// 		 else if (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES|Q_PATQUOTE))
// 	   {
// 		 /* If we have "$*" we want to make a string of the positional
// 			parameters, separated by the first character of $IFS, and
// 			quote the whole string, including the separators.  If IFS
// 			is unset, the parameters are separated by ' '; if $IFS is
// 			null, the parameters are concatenated. */
// 		 temp = (quoted & (Q_DOUBLE_QUOTES|Q_PATQUOTE)) ? string_list_dollar_star (list, quoted, 0) : string_list (list);
// 		 if (temp)
// 		   {
// 			 temp1 = (quoted & Q_DOUBLE_QUOTES) ? quote_string (temp) : temp;
// 			 if (*temp == 0)
// 		   tflag |= W_HASQUOTEDNULL;
// 			 if (temp != temp1)
// 		   free (temp);
// 			 temp = temp1;
// 		   }
// 	   }
// 		 else
// 	   {
// 		 /* We check whether or not we're eventually going to split $* here,
// 			for example when IFS is empty and we are processing the rhs of
// 			an assignment statement.  In that case, we don't separate the
// 			arguments at all.  Otherwise, if the $* is not quoted it is
// 			identical to $@ */
// 		 if (expand_no_split_dollar_star && quoted == 0 && ifs_is_set == 0 && (pflags & PF_ASSIGNRHS))
// 		   {
// 			 /* Posix interp 888: RHS of assignment, IFS unset: no splitting,
// 			separate with space */
// 			 temp1 = string_list_dollar_star (list, quoted, pflags);
// 			 temp = temp1 ? quote_string (temp1) : temp1;
// 			 /* XXX - tentative - note that we saw a quoted null here */
// 			 if (temp1 && *temp1 == 0 && QUOTED_NULL (temp))
// 		   tflag |= W_SAWQUOTEDNULL;
// 			 FREE (temp1);
// 		   }
// 		 else if (expand_no_split_dollar_star && quoted == 0 && ifs_is_null && (pflags & PF_ASSIGNRHS))
// 		   {
// 			 /* Posix interp 888: RHS of assignment, IFS set to '' */
// 			 temp1 = string_list_dollar_star (list, quoted, pflags);
// 			 temp = temp1 ? quote_escapes (temp1) : temp1;
// 			 FREE (temp1);
// 		   }
// 		 else if (expand_no_split_dollar_star && quoted == 0 && ifs_is_set && ifs_is_null == 0 && (pflags & PF_ASSIGNRHS))
// 		   {
// 			 /* Posix interp 888: RHS of assignment, IFS set to non-null value */
// 			 temp1 = string_list_dollar_star (list, quoted, pflags);
// 			 temp = temp1 ? quote_string (temp1) : temp1;

// 			 /* XXX - tentative - note that we saw a quoted null here */
// 			 if (temp1 && *temp1 == 0 && QUOTED_NULL (temp))
// 		   tflag |= W_SAWQUOTEDNULL;
// 			 FREE (temp1);
// 		   }
// 		 /* XXX - should we check ifs_is_set here as well? */
//    #  if defined (HANDLE_MULTIBYTE)
// 		 else if (expand_no_split_dollar_star && ifs_firstc[0] == 0)
//    #  else
// 		 else if (expand_no_split_dollar_star && ifs_firstc == 0)
//    #  endif
// 		   /* Posix interp 888: not RHS, no splitting, IFS set to '' */
// 		   temp = string_list_dollar_star (list, quoted, 0);
// 		 else
// 		   {
// 			 temp = string_list_dollar_at (list, quoted, 0);
// 			 /* Set W_SPLITSPACE to make sure the individual positional
// 			parameters are split into separate arguments */
//    #if 0
// 			 if (quoted == 0 && (ifs_is_set == 0 || ifs_is_null))
//    #else	/* change with bash-5.0 */
// 			 if (quoted == 0 && ifs_is_null)
//    #endif
// 		   tflag |= W_SPLITSPACE;
// 			 /* If we're not quoted but we still don't want word splitting, make
// 			we quote the IFS characters to protect them from splitting (e.g.,
// 			when $@ is in the string as well). */
// 			 else if (temp && quoted == 0 && ifs_is_set && (pflags & PF_ASSIGNRHS))
// 		   {
// 			 temp1 = quote_string (temp);
// 			 free (temp);
// 			 temp = temp1;
// 		   }
// 		   }

// 		 if (expand_no_split_dollar_star == 0 && contains_dollar_at)
// 		   *contains_dollar_at = 1;
// 	   }

// 		 dispose_words (list);
// 		 break;

// 	   /* When we have "$@" what we want is "$1" "$2" "$3" ... This
// 		  means that we have to turn quoting off after we split into
// 		  the individually quoted arguments so that the final split
// 		  on the first character of $IFS is still done.  */
// 	   case '@':		/* `$@' */
// 		 list = list_rest_of_args ();
// 		 for (nullarg = 0, l = list; l; l = l->next)
// 	   {
// 		 if (l->word && (l->word->word == 0 || l->word->word[0] == 0))
// 		   nullarg = 1;
// 	   }

// 		 /* We want to flag the fact that we saw this.  We can't turn
// 		off quoting entirely, because other characters in the
// 		string might need it (consider "\"$@\""), but we need some
// 		way to signal that the final split on the first character
// 		of $IFS should be done, even though QUOTED is 1. */
// 		 /* XXX - should this test include Q_PATQUOTE? */
// 		 if (quoted_dollar_at_p && (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)))
// 	   *quoted_dollar_at_p = 1;
// 		 if (contains_dollar_at)
// 	   *contains_dollar_at = 1;

// 		 /* We want to separate the positional parameters with the first
// 		character of $IFS in case $IFS is something other than a space.
// 		We also want to make sure that splitting is done no matter what --
// 		according to POSIX.2, this expands to a list of the positional
// 		parameters no matter what IFS is set to. */
// 		 /* XXX - what to do when in a context where word splitting is not
// 		performed? Even when IFS is not the default, posix seems to imply
// 		that we have to expand $@ to all the positional parameters and
// 		separate them with spaces, which are preserved because word splitting
// 		doesn't take place.  See below for how we use PF_NOSPLIT2 here. */

// 		 /* These are the cases where word splitting will not be performed. */
// 		 if (pflags & PF_ASSIGNRHS)
// 	   {
// 		 temp = string_list_dollar_at (list, (quoted|Q_DOUBLE_QUOTES), pflags);
// 		 if (nullarg)
// 		   tflag |= W_HASQUOTEDNULL;	/* we know quoting produces quoted nulls */
// 	   }

// 		 /* This needs to match what expand_word_internal does with non-quoted $@
// 		does with separating with spaces.  Passing Q_DOUBLE_QUOTES means that
// 		the characters in LIST will be quoted, and PF_ASSIGNRHS ensures that
// 		they will separated by spaces. After doing this, we need the special
// 		handling for PF_NOSPLIT2 in expand_word_internal to remove the CTLESC
// 		quotes. */
// 		 else if (pflags & PF_NOSPLIT2)
// 		   {
//    #if defined (HANDLE_MULTIBYTE)
// 		 if (quoted == 0 && ifs_is_set && ifs_is_null == 0 && ifs_firstc[0] != ' ')
//    #else
// 		 if (quoted == 0 && ifs_is_set && ifs_is_null == 0 && ifs_firstc != ' ')
//    #endif
// 		   /* Posix interp 888 */
// 		   temp = string_list_dollar_at (list, Q_DOUBLE_QUOTES, pflags);
// 		 else
// 		   temp = string_list_dollar_at (list, quoted, pflags);
// 	   }
// 		 else
// 	   temp = string_list_dollar_at (list, quoted, pflags);

// 		 tflag |= W_DOLLARAT;
// 		 dispose_words (list);
// 		 break;

// 	   case LBRACE:
// 		 tdesc = parameter_brace_expand (string, &zindex, quoted, pflags,
// 						 quoted_dollar_at_p,
// 						 contains_dollar_at);

// 		 if (tdesc == &expand_wdesc_error || tdesc == &expand_wdesc_fatal)
// 	   return (tdesc);
// 		 temp = tdesc ? tdesc->word : (char *)0;

// 		 /* XXX */
// 		 /* Quoted nulls should be removed if there is anything else
// 		in the string. */
// 		 /* Note that we saw the quoted null so we can add one back at
// 		the end of this function if there are no other characters
// 		in the string, discard TEMP, and go on.  The exception to
// 		this is when we have "${@}" and $1 is '', since $@ needs
// 		special handling. */
// 		 if (tdesc && tdesc->word && (tdesc->flags & W_HASQUOTEDNULL) && QUOTED_NULL (temp))
// 	   {
// 		 if (had_quoted_null_p)
// 		   *had_quoted_null_p = 1;
// 		 if (*quoted_dollar_at_p == 0)
// 		   {
// 			 free (temp);
// 			 tdesc->word = temp = (char *)NULL;
// 		   }

// 	   }

// 		 ret = tdesc;
// 		 goto return0;

// 	   /* Do command or arithmetic substitution. */
// 	   case LPAREN:
// 		 /* We have to extract the contents of this paren substitution. */
// 		 t_index = zindex + 1;
// 		 /* XXX - might want to check for string[t_index+2] == LPAREN and parse
// 		as arithmetic substitution immediately. */
// 		 temp = extract_command_subst (string, &t_index, (pflags&PF_COMPLETE) ? SX_COMPLETE : 0);
// 		 zindex = t_index;

// 		 /* For Posix.2-style `$(( ))' arithmetic substitution,
// 		extract the expression and pass it to the evaluator. */
// 		 if (temp && *temp == LPAREN)
// 	   {
// 		 char *temp2;
// 		 temp1 = temp + 1;
// 		 temp2 = savestring (temp1);
// 		 t_index = strlen (temp2) - 1;

// 		 if (temp2[t_index] != RPAREN)
// 		   {
// 			 free (temp2);
// 			 goto comsub;
// 		   }

// 		 /* Cut off ending `)' */
// 		 temp2[t_index] = '\0';

// 		 if (chk_arithsub (temp2, t_index) == 0)
// 		   {
// 			 free (temp2);
//    #if 0
// 			 internal_warning (_("future versions of the shell will force evaluation as an arithmetic substitution"));
//    #endif
// 			 goto comsub;
// 		   }

// 		 /* Expand variables found inside the expression. */
// 		 temp1 = expand_arith_string (temp2, Q_DOUBLE_QUOTES|Q_ARITH);
// 		 free (temp2);

//    arithsub:
// 		 /* No error messages. */
// 		 savecmd = this_command_name;
// 		 this_command_name = (char *)NULL;

// 		 eflag = (shell_compatibility_level > 51) ? 0 : EXP_EXPANDED;
// 		 number = evalexp (temp1, eflag, &expok);
// 		 this_command_name = savecmd;
// 		 free (temp);
// 		 free (temp1);
// 		 if (expok == 0)
// 		   {
// 			 if (interactive_shell == 0 && posixly_correct)
// 		   {
// 			 set_exit_status (EXECUTION_FAILURE);
// 			 return (&expand_wdesc_fatal);
// 		   }
// 			 else
// 		   return (&expand_wdesc_error);
// 		   }
// 		 temp = itos (number);
// 		 break;
// 	   }

//    comsub:
// 		 if (pflags & PF_NOCOMSUB)
// 	   /* we need zindex+1 because string[zindex] == RPAREN */
// 	   temp1 = substring (string, *sindex, zindex+1);
// 		 else
// 	   {
// 		 tdesc = command_substitute (temp, quoted, pflags&PF_ASSIGNRHS);
// 		 temp1 = tdesc ? tdesc->word : (char *)NULL;
// 		 if (tdesc)
// 		   dispose_word_desc (tdesc);
// 	   }
// 		 FREE (temp);
// 		 temp = temp1;
// 		 break;

// 	   /* Do POSIX.2d9-style arithmetic substitution.  This will probably go
// 		  away in a future bash release. */
// 	   case '[':		/*]*/
// 		 /* Extract the contents of this arithmetic substitution. */
// 		 t_index = zindex + 1;
// 		 temp = extract_arithmetic_subst (string, &t_index);
// 		 zindex = t_index;
// 		 if (temp == 0)
// 	   {
// 		 temp = savestring (string);
// 		 if (expanded_something)
// 		   *expanded_something = 0;
// 		 goto return0;
// 	   }

// 		  /* Do initial variable expansion. */
// 		 temp1 = expand_arith_string (temp, Q_DOUBLE_QUOTES|Q_ARITH);

// 		 goto arithsub;

// 	   default:
// 		 /* Find the variable in VARIABLE_LIST. */
// 		 temp = (char *)NULL;

// 		 for (t_index = zindex; (c = string[zindex]) && legal_variable_char (c); zindex++)
// 	   ;
// 		 temp1 = (zindex > t_index) ? substring (string, t_index, zindex) : (char *)NULL;

// 		 /* If this isn't a variable name, then just output the `$'. */
// 		 if (temp1 == 0 || *temp1 == '\0')
// 	   {
// 		 FREE (temp1);
// 		 temp = (char *)xmalloc (2);
// 		 temp[0] = '$';
// 		 temp[1] = '\0';
// 		 if (expanded_something)
// 		   *expanded_something = 0;
// 		 goto return0;
// 	   }

// 		 /* If the variable exists, return its value cell. */
// 		 var = find_variable (temp1);

// 		 if (var && invisible_p (var) == 0 && var_isset (var))
// 	   {
//    #if defined (ARRAY_VARS)
// 		 if (assoc_p (var) || array_p (var))
// 		   {
// 			 temp = array_p (var) ? array_reference (array_cell (var), 0)
// 					  : assoc_reference (assoc_cell (var), "0");
// 			 if (temp)
// 		   temp = (*temp && (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)))
// 				 ? quote_string (temp)
// 				 : quote_escapes (temp);
// 			 else if (unbound_vars_is_error)
// 		   goto unbound_variable;
// 		   }
// 		 else
//    #endif
// 		   {
// 			 temp = value_cell (var);

// 			 temp = (*temp && (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)))
// 			   ? quote_string (temp)
// 			   : ((pflags & PF_ASSIGNRHS) ? quote_rhs (temp)
// 							  : quote_escapes (temp));
// 		   }

// 		 free (temp1);

// 		 goto return0;
// 	   }
// 		 else if (var && (invisible_p (var) || var_isset (var) == 0))
// 	   temp = (char *)NULL;
// 		 else if ((var = find_variable_last_nameref (temp1, 0)) && var_isset (var) && invisible_p (var) == 0)
// 	   {
// 		 temp = nameref_cell (var);
//    #if defined (ARRAY_VARS)
// 		 if (temp && *temp && valid_array_reference (temp, 0))
// 		   {
// 			 chk_atstar (temp, quoted, pflags, quoted_dollar_at_p, contains_dollar_at);
// 			 tdesc = parameter_brace_expand_word (temp, SPECIAL_VAR (temp, 0), quoted, pflags, 0);
// 			 if (tdesc == &expand_wdesc_error || tdesc == &expand_wdesc_fatal)
// 		   return (tdesc);
// 			 ret = tdesc;
// 			 goto return0;
// 		   }
// 		 else
//    #endif
// 		 /* y=2 ; typeset -n x=y; echo $x is not the same as echo $2 in ksh */
// 		 if (temp && *temp && legal_identifier (temp) == 0)
// 		   {
// 			 set_exit_status (EXECUTION_FAILURE);
// 			 report_error (_("%s: invalid variable name for name reference"), temp);
// 			 return (&expand_wdesc_error);	/* XXX */
// 		   }
// 		 else
// 		   temp = (char *)NULL;
// 	   }

// 		 temp = (char *)NULL;

//    unbound_variable:
// 		 if (unbound_vars_is_error)
// 	   {
// 		 set_exit_status (EXECUTION_FAILURE);
// 		 err_unboundvar (temp1);
// 	   }
// 		 else
// 	   {
// 		 free (temp1);
// 		 goto return0;
// 	   }

// 		 free (temp1);
// 		 set_exit_status (EXECUTION_FAILURE);
// 		 return ((unbound_vars_is_error && interactive_shell == 0)
// 		   ? &expand_wdesc_fatal
// 		   : &expand_wdesc_error);
// 	   }

// 	 if (string[zindex])
// 	   zindex++;

//    return0:
// 	 *sindex = zindex;

// 	 if (ret == 0)
// 	   {
// 		 ret = alloc_word_desc ();
// 		 ret->flags = tflag;	/* XXX */
// 		 ret->word = temp;
// 	   }
// 	return ret;
// }

// unsigned int	hash_string(const char *s)
// {
// 	unsigned int i;

// 	i = FNV_OFFSET;
// 	while (*s)
// 	{
// 		i += (i<<1) + (i<<4) + (i<<7) + (i<<8) + (i<<24);
// 		i ^= *s;
// 		s++;
// 	}
// 	return (i);
// }

// t_bucket	*hash_search(const char *string, t_hash table, int flags)
// {
// 	t_bucket		*list;
// 	int				bucket;
// 	unsigned int	hv;

// 	if (!table || !table->nentries)
// 		return (NULL);
// 	hv = hash_string(string);
// 	bucket = (hv & (table->nbuckets - 1));
// 	list = table->bucket_array ? table->bucket_array[bucket] : 0;
// 	while (list)
// 	{
// 		if (hv == list->khash && ft_strncmp(list->key, string) == 0)
// 		{
// 			list->times_found++;
// 			return (list);
// 		}
// 		list = list->next;
// 	}
// 	return (NULL);
// }

// t_variable	*hash_lookup(const char *name, t_hash *hashed_vars)
// {
// 	t_bucket *bucket;

// 	bucket = hash_search(name, hashed_vars, 0);
// 	if (bucket)
// 		last_table_searched = hashed_vars;
// 	return (bucket ? (t_variable *)bucket->data : (t_variable *)NULL);
// }

// t_variable	*find_variable_internal(const char *name)
// {
// 	t_variable	*var;

// 	last_table_searched = 0;
// 	var = NULL;
// 	if (temporary_env)
// 		var = hash_lookup(name, temporary_env);
// 	if (!var)
// 		return (NULL);
// 	return (var);
// }

// t_word	*param_expand(char *string, int *sindex, int quoted, int *expanded_something, int *contains_dollar_at, int *quoted_dollar_at_p, int *had_quoted_null_p, int pflags)
// {
// 	char		*temp;
// 	char		*temp1;
// 	int			zindex;
// 	int			t_index;
// 	char		c;
// 	t_variable	*var;
// 	t_word		*ret;

// 	zindex = *sindex;
// 	c = string[++zindex];
// 	temp = NULL;
// 	ret = NULL;
// 	if (c == '?')
// 		 temp = itos(last_command_exit_value);
// 	else
// 	{
// 		temp = NULL;
// 		t_index = zindex;
// 		while ((c = string[zindex]) && (ft_isalnum(c) || c == '_'))
// 			zindex++;
// 		temp1 = (zindex > t_index) ? ft_substr(string, t_index, zindex) : NULL;
// 		if (!temp1 || !*temp1)
// 		{
// 			temp = ft_strdup("$");
// 			if (expanded_something)
// 				*expanded_something = 0;
// 			goto return0;
// 		}
// 		var = find_variable_internal(temp1);
// 		if (var && var->value)
// 		{
// 			temp = var->value;
// 			goto return0;
// 		}
// 		temp = NULL;
// 		if (unbound_vars_is_error)
// 		{
// 			set_exit_status (EXECUTION_FAILURE);
// 			err_unboundvar (temp1);
// 		}
// 		else
// 			goto return0;
// 		set_exit_status (EXECUTION_FAILURE);
// 		return (NULL);
// 	}
// 	if (string[zindex])
// 		zindex++;
// 	return0:
// 	*sindex = zindex;
// 	if (ret == 0)
// 	{
// 		ret = alloc_word_desc();
// 		ret->flags = 0;
// 		ret->word = temp;
// 	}
// 	return ret;
// }
