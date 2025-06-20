/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:01:10 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 16:14:17 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "makers.h"
#include "libft.h"
#include "expander.h"

t_word_list	*expand_word_internal(t_word *word, int quoted, int isexp, int *contains_dollar_at, int *expanded_something)
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
	int				has_dollar_at;
	int				temp_has_dollar_at;
	int				internal_tilde;
	int				split_on_spaces;
	int				local_expanded;
	int				tflag;
	int				pflags;
	int				assignoff;
	unsigned char	c;
	int				t_index;
	char			twochars[2];
	DECLARE_MBSTATE;

	if (STREQ (word->word, "\"$@\"") && (word->flags == (W_HASDOLLAR|W_QUOTED)) && dollar_vars[1])
	{
		if (contains_dollar_at)
			*contains_dollar_at = 1;
		if (expanded_something)
			*expanded_something = 1;
		if (cached_quoted_dollar_at)
			return (copy_word_list (cached_quoted_dollar_at));
		list = list_rest_of_args ();
		list = quote_list (list);
		cached_quoted_dollar_at = copy_word_list (list);
		return (list);
	}
	istring_size = DEFAULT_INITIAL_ARRAY_SIZE
	istring = (char *)xmalloc(istring_size);
	istring_index = 0;
	istring[istring_index] = '\0';
	quoted_dollar_at = 0;
	had_quoted_null = 0;
	has_dollar_at = 0;
	has_quoted_ifs = 0;
	split_on_spaces = 0;
	internal_tilde = 0;
	quoted_state = UNQUOTED;
	string = word->word;
	if (string == 0)
		goto finished_with_string;

	if (MB_CUR_MAX > 1)
		string_size = strlen(string);
	else
		string_size = 1;
	if (contains_dollar_at)
		*contains_dollar_at = 0;
	assignoff = -1;
	sindex = 0;
	while (1)
	{
		c = string[sindex];
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
					case '<':
					case '>':
					{
						if (string[++sindex] != LPAREN || (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) || (word->flags & W_NOPROCSUB))
						{
						sindex--;
						goto add_character;
						}
						else
						t_index = sindex + 1;
						temp1 = extract_process_subst (string, (c == '<') ? "<(" : ">(", &t_index, 0);
						sindex = t_index;
						temp = temp1 ? process_substitute (temp1, (c == '>')) : (char *)0;
						FREE (temp1);
						goto dollar_add_string;
					}
				#endif

				#if defined (ARRAY_VARS)
					case '[':
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
				if (word->flags & (W_ASSIGNRHS|W_NOTILDE))
				{
					if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
						goto add_ifs_character;
					else
						goto add_character;
				}
				if ((word->flags & W_ASSIGNMENT) && (posixly_correct == 0 || (word->flags & W_TILDEEXP)) && assignoff == -1 && sindex > 0)
					assignoff = sindex;
				if (sindex == assignoff && string[sindex+1] == '~')
					internal_tilde = 1;
				if (word->flags & W_ASSIGNARG)
					word->flags |= W_ASSIGNRHS;
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
				if ((word->flags & (W_ASSIGNMENT|W_ASSIGNRHS)) && (posixly_correct == 0 || (word->flags & W_TILDEEXP)) && string[sindex+1] == '~')
					internal_tilde = 1;
				if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && isifs (c))
					goto add_ifs_character;
				else
					goto add_character;

			case '~':
				if ((word->flags & W_NOTILDE) || (sindex > 0 && (internal_tilde == 0)) || (quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)))
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
						goto add_character;
					}
					free (temp);
					temp = temp1;
					sindex += t_index;
					goto add_quoted_string;
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
					return ((tword == &expand_wdesc_error) ? &expand_word_error : &expand_word_fatal);
				}
				if (contains_dollar_at && has_dollar_at)
					*contains_dollar_at = 1;
				if (tword && (tword->flags & W_HASQUOTEDNULL))
					had_quoted_null = 1;
				if (tword && (tword->flags & W_SAWQUOTEDNULL))
					had_quoted_null = 1;
				temp = tword ? tword->word : (char *)NULL;
				dispose_word_desc (tword);
				if (had_quoted_null && temp && QUOTED_NULL (temp))
				{
					FREE (temp);
					temp = (char *)NULL;
				}
				goto add_string;
				break;
			case '`':
			{
				t_index = sindex++;
				temp = string_extract (string, &sindex, "`", (word->flags & W_COMPLETE) ? SX_COMPLETE : SX_REQMATCH);
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
					return ((temp == &extract_string_error) ? &expand_word_error : &expand_word_fatal);
				}
				if (expanded_something)
				*expanded_something = 1;
				local_expanded = 1;
				if (word->flags & W_NOCOMSUB)
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
				if ((quoted & Q_HERE_DOCUMENT) && (quoted & Q_DOLBRACE) && c == '"')
					tflag = CBSDQUOTE;		/* special case */
				else if (quoted & Q_HERE_DOCUMENT)
					tflag = CBSHDOC;
				else if (quoted & Q_DOUBLE_QUOTES)
					tflag = CBSDQUOTE;
				else
					tflag = 0;
				if ((quoted & Q_DOLBRACE) && c == RBRACE)
					SCOPY_CHAR_I (twochars, CTLESC, c, string, sindex, string_size);
				else if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && ((sh_syntaxtab[c] & tflag) == 0) && isexp == 0 && isifs (c))
				{
					RESIZE_MALLOCED_BUFFER (istring, istring_index, 2, istring_size, DEFAULT_ARRAY_SIZE);
					istring[istring_index++] = CTLESC;
					istring[istring_index++] = '\\';
					istring[istring_index] = '\0';
					SCOPY_CHAR_I (twochars, CTLESC, c, string, sindex, string_size);
				}
				else if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && c == 0)
				{
					RESIZE_MALLOCED_BUFFER (istring, istring_index, 2, istring_size, DEFAULT_ARRAY_SIZE);
					istring[istring_index++] = CTLESC;
					istring[istring_index++] = '\\';
					istring[istring_index] = '\0';
					break;
				}
				else if ((quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES)) && ((sh_syntaxtab[c] & tflag) == 0))
					SCOPY_CHAR_I (twochars, '\\', c, string, sindex, string_size);
				else if (c == 0)
				{
					c = CTLNUL;
					sindex--;
					goto add_character;
				}
				else
				{
					SCOPY_CHAR_I (twochars, CTLESC, c, string, sindex, string_size);
				}

				sindex++;
				add_twochars:
				RESIZE_MALLOCED_BUFFER (istring, istring_index, 2, istring_size, DEFAULT_ARRAY_SIZE);
				istring[istring_index++] = twochars[0];
				istring[istring_index++] = twochars[1];
				istring[istring_index] = '\0';
				break;
			case '"':

				if ((quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)) && ((quoted & Q_ARITH) == 0))
					goto add_character;
				t_index = ++sindex;
				temp = string_extract_double_quoted (string, &sindex, (word->flags & W_COMPLETE) ? SX_COMPLETE : 0);
				quoted_state = (t_index == 1 && string[sindex] == '\0') ? WHOLLY_QUOTED : PARTIALLY_QUOTED;
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
					temp_has_dollar_at = 0;
					list = expand_word_internal (tword, Q_DOUBLE_QUOTES|(quoted&Q_ARITH), 0, &temp_has_dollar_at, (int *)NULL);
					has_dollar_at += temp_has_dollar_at;
					if (list == &expand_word_error || list == &expand_word_fatal)
					{
						free (istring);
						free (string);
						tword->word = (char *)NULL;
						dispose_word (tword);
						return list;
					}
					dispose_word (tword);
					if (list == 0 && temp_has_dollar_at)
					{
						quoted_dollar_at++;
						break;
					}
					if (list && list->word && list->next == 0 && (list->word->flags & W_HASQUOTEDNULL))
					{
						if (had_quoted_null && temp_has_dollar_at) quoted_dollar_at++;
							had_quoted_null = 1;
					}
					if (list)
						dequote_list (list);
					if (temp_has_dollar_at)
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
					FREE (temp);
					list = (WORD_LIST *)NULL;
					had_quoted_null = 1;
				}
				if (list)
				{
					if (list->next)
					{
						temp = quoted_dollar_at ? string_list_dollar_at (list, Q_DOUBLE_QUOTES, 0) : string_list (quote_list (list));
						dispose_words (list);
						goto add_string;
					}
					else
					{
						temp = savestring (list->word->word);
						tflag = list->word->flags;
						dispose_words (list);
						if ((tflag & W_HASQUOTEDNULL) && QUOTED_NULL (temp) == 0)
							remove_quoted_nulls (temp);
					}
				}
				else
					temp = (char *)NULL;
				if (temp == 0 && quoted_state == PARTIALLY_QUOTED)
					had_quoted_null = 1;
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

					c = CTLNUL;
					sindex--;
					had_quoted_null = 1;
					goto add_character;
				}
			case '\'':
				if ((quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)))
					goto add_character;
				t_index = ++sindex;
				temp = string_extract_single_quoted (string, &sindex, 0);
				quoted_state = (t_index == 1 && string[sindex] == '\0') ? WHOLLY_QUOTED : PARTIALLY_QUOTED;
				if (*temp == '\0')
				{
					free (temp);
					temp = (char *)NULL;
				}
				else
					remove_quoted_escapes (temp);
				if (temp == 0 && quoted_state == PARTIALLY_QUOTED)
					had_quoted_null = 1;
				if (temp == 0 && quoted_state == PARTIALLY_QUOTED && quoted == 0 && (word->flags & (W_NOSPLIT|W_EXPANDRHS|W_ASSIGNRHS)) == W_EXPANDRHS)
				{
					c = CTLNUL;
					sindex--;
					goto add_character;
				}
				if (temp == 0 && (quoted_state == PARTIALLY_QUOTED) && (word->flags & (W_NOSPLIT|W_NOSPLIT2)))
					continue;
				if (temp == 0)
				{
					c = CTLNUL;
					sindex--;
					goto add_character;
				}
				else
					goto add_quoted_string;
			case ' ':
				if (ifs_is_null || split_on_spaces || ((word->flags & (W_NOSPLIT|W_NOSPLIT2|W_ASSIGNRHS)) && (word->flags & W_EXPANDRHS) == 0))
				{
					if (string[sindex])
						sindex++;
					twochars[0] = CTLESC;
					twochars[1] = c;
					goto add_twochars;
				}
			default:
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
						if (mb_cur_max > 1)
							sindex--;
						if (mb_cur_max > 1)
							SADD_MBQCHAR_BODY(temp, string, sindex, string_size);
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
				RESIZE_MALLOCED_BUFFER (istring, istring_index, 1, istring_size, DEFAULT_ARRAY_SIZE);
				istring[istring_index++] = c;
				istring[istring_index] = '\0';
				sindex++;
		}
	}

	finished_with_string:
	if (*istring == '\0')
	{
		if (had_quoted_null || (quoted_dollar_at == 0 && quoted_state == PARTIALLY_QUOTED))
		{
			istring[0] = CTLNUL;
			istring[1] = '\0';
			tword = alloc_word_desc ();
			tword->word = istring;
			istring = 0;
			tword->flags |= W_HASQUOTEDNULL;
			list = make_word_list (tword, (WORD_LIST *)NULL);
			if (quoted & (Q_HERE_DOCUMENT|Q_DOUBLE_QUOTES))
				tword->flags |= W_QUOTED;
		}
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
		istring = 0;
		if (word->flags & W_ASSIGNMENT)
		tword->flags |= W_ASSIGNMENT;
		if (word->flags & W_COMPASSIGN)
		tword->flags |= W_COMPASSIGN;	/* XXX */
		if (word->flags & W_NOGLOB)
		tword->flags |= W_NOGLOB;	/* XXX */
		if (word->flags & W_NOBRACE)
		tword->flags |= W_NOBRACE;
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
		istring = 0;
		goto set_word_flags;
	}
	else
	{
		char *ifs_chars;
		ifs_chars = (quoted_dollar_at || has_dollar_at) ? ifs_value : (char *)NULL;
		if (split_on_spaces)
		{
			if (ifs_is_set == 0)
				list = list_string (istring, " \t\n", 1);
			else
				list = list_string (istring, " ", 1);
		}
		else if (has_dollar_at && quoted_dollar_at == 0 && ifs_chars && quoted == 0 && (word->flags & W_NOSPLIT2))
		{
			tword = alloc_word_desc ();
			if (*ifs_chars && *ifs_chars != ' ')
			{
				list = list_string (istring, *ifs_chars ? ifs_chars : " ", 1);
				tword->word = string_list (list);
			}
			else
				tword->word = istring;
			if (had_quoted_null && QUOTED_NULL (istring))
				tword->flags |= W_HASQUOTEDNULL;
			if (tword->word != istring)
				free (istring);
			istring = 0;
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
			if (had_quoted_null && QUOTED_NULL (istring))
				tword->flags |= W_HASQUOTEDNULL;
			else if (had_quoted_null)
				tword->flags |= W_SAWQUOTEDNULL;
			if (tword->word != istring)
				free (istring);
			istring = 0;
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
