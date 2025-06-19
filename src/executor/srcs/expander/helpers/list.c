/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:04:08 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/17 23:15:30 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "makers.h"
#include "utils.h"
#include "token.h"

t_word_list *list_append(t_word_list *head, t_word_list *tail)
{
	t_word_list	*t_head;

	if (!head)
		return (tail);
	t_head = head;
	while (t_head->next)
		t_head = t_head->next;
	t_head->next = tail;
	return (head);
}

t_word_list	*list_string(char *string, char *separators, int quoted)
{
	t_word_list	*result;
	t_word		*t;
	char		*current_word;
	char		*s;
	int			sindex;
	int			whitesep;
	int			xflags;
	size_t		slen;

	if (!string || !*string)
		return (NULL);
	xflags = 0;
	slen = 0;
	if (!quoted && separators && *separators)
	{
		s = string;
		while (*s && (separators[1] ? 1 : *s == separators[0]) && ft_isspace(*s))
			s++;
		if (!*s)
			return (NULL);
		string = s;
	}
	slen = ft_strlen(string);
	result = NULL;
	sindex = 0;
	while (string[sindex])
	{
		current_word = string_extract_verbatim (string, slen, &sindex, separators, xflags);
		if (!current_word)
			break ;
		if (current_word[0])
		{
			t = alloc_word_desc ();
			t->word = current_word;
			result = make_word_list(t, result);
			if (quoted)
				result->word->flags |= F_QUOTED;
		}
		whitesep = string[sindex] && ft_isspace(string[sindex]);
		if (string[sindex])
			sindex += ft_mbrlen(string + sindex, slen - sindex);
		while (string[sindex] && ft_isspace(string[sindex]) && (separators[1] ? 1 : *s == separators[0]))
			sindex++;
		if (string[sindex] && whitesep && i(separators[1] ? 1 : *s == separators[0]) && !ft_isspace (string[sindex]))
		{
			sindex++;
			while (string[sindex] && ft_isspace (string[sindex]))
				sindex++;
		}
	}
	return (ft_revword(result));
}

t_word_list	*word_split(t_word *w)
{
	t_word_list	*result;

	if (w)
		result = list_string(w->word, " \t\n", w->flags & F_QUOTED);
	else
		result = NULL;
	return (result);
}

t_word_list	*word_list_split(t_word_list *list)
{
	t_word_list	*result;
	t_word_list	*t;
	t_word_list	*tresult;
	t_word_list	*e;
	t_word		*w;

	t = list;
	result = NULL;
	while (t)
	{
		tresult = word_split(t->word);
		if (!tresult && t->word)
		{
			w = alloc_word_desc();
			w->word = ft_strdup("");
			tresult = make_word_list(w, NULL);
		}
		if (!result)
		{
			e = tresult;
			result = e;
		}
		else
		{
			e->next = tresult;
			while (e->next)
				e = e->next;
		}
		t = t->next;
	}
	return (result);
}
