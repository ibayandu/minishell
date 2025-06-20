/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 05:51:55 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 06:41:07 by yzeybek          ###   ########.tr       */
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
	sindex = 0;
	while (1)
	{
		c = string[sindex];
		switch (c)
		{
			case '\0':
				goto finished_with_string;
			case '=':
				if (word->flags & (W_ASSIGNRHS|W_NOTILDE))
				{
					if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && c == ' ')
						goto add_ifs_character;
					else
						goto add_character;
				}
				if (word->flags & W_ASSIGNMENT && assignoff == -1 && sindex > 0)
					assignoff = sindex;
				if (word->flags & W_ASSIGNARG)
					word->flags |= W_ASSIGNRHS;		/* affects $@ */
				if (isexp == 0 && (word->flags & (W_NOSPLIT|W_NOSPLIT2)) == 0 && c == ' ')
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
				split_on_spaces += (tword->flags & W_SPLITSPACE);
				if (tword == &expand_wdesc_error || tword == &expand_wdesc_fatal)
					return ((tword == &expand_wdesc_error) ? &expand_word_error
										: &expand_word_fatal);
				temp = tword ? tword->word : (char *)NULL;
				/* Kill quoted nulls; we will add them back at the end of
					expand_word_internal if nothing else in the string */
				if (had_quoted_null && temp && QUOTED_NULL (temp))
					{
					FREE (temp);
					temp = (char *)NULL;
					}

				goto add_string;
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
				temp = NULL;
				list = expand_word_internal (tword, Q_DOUBLE_QUOTES|(quoted&Q_ARITH), 0, (int *)NULL);
				if (list == &expand_word_error || list == &expand_word_fatal)
				{
					tword->word = NULL;
					return list;
				}
			}
			else
				list = NULL;
			if (list)
			{
				if (list->next)
				{
					temp = string_list (quote_list (list));
					goto add_string;
				}
				else
				{
					temp = savestring (list->word->word);
					tflag = list->word->flags;
				}
			}
			else
				temp = (char *)NULL;
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
			case '\'':
				if ((quoted & (Q_DOUBLE_QUOTES|Q_HERE_DOCUMENT)))
					goto add_character;

				t_index = ++sindex;
				string_extract_single_quoted (string, &sindex, &temp);
				quoted_state = (t_index == 1 && string[sindex] == '\0') ? WHOLLY_QUOTED : PARTIALLY_QUOTED;

				if (*temp == '\0')
					temp = NULL;
				else
					remove_quoted_escapes (temp);
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
				if (split_on_spaces || ((word->flags & (W_NOSPLIT|W_NOSPLIT2|W_ASSIGNRHS)) && (word->flags & W_EXPANDRHS) == 0))
				{
					if (string[sindex])
						sindex++;
					twochars[0] = CTLESC;
					twochars[1] = c;
					goto add_twochars;
				}
				/* FALLTHROUGH */

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

	if (*istring == '\0')
		list = NULL;
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

		ifs_chars = NULL;

		if (split_on_spaces)
			list = list_string (istring, " \t\n", 1);

		else if (ifs_chars && quoted == 0 && (word->flags & W_NOSPLIT2))
		{
			tword = alloc_word_desc ();
			if (*ifs_chars && *ifs_chars != ' ')
			{
				list = list_string (istring, *ifs_chars ? ifs_chars : " ", 1);
				tword->word = string_list (list);
			}
			else
				tword->word = istring;
			istring = 0;
			goto set_word_flags;
		}
		else if (ifs_chars)
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
	return (list);
}
