/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:04:08 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 12:37:31 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "makers.h"
#include "utils.h"
#include "token.h"
#include "expander.h"

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

t_word_list	*list_string(char *string, int quoted)
{
	t_word_list *result;
	t_word	*t;
	char *current_word;
	char *s;
	int sindex;
	size_t slen;

	if (!string || !*string)
		return (NULL);
	slen = 0;
	if (!quoted)
	{
		s = string;
		while (*s && ft_strchr(" \t\n", *s))
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
		current_word = string_extract_verbatim(string, slen, &sindex, " \t\n");
		if (!current_word)
			break;
		if (*current_word)
		{
			t = alloc_word_desc ();
			t->word = current_word;
			result = make_word_list(t, result);
			if (quoted)
				result->word->flags |= F_QUOTED;
		}
		while (string[sindex] && ft_strchr(" \t\n", string[sindex]))
			sindex++;
	}
	return (ft_revword(result));
}

// static size_t	count_word(char const *s, char c)
// {
// 	size_t	i;
// 	size_t	count;

// 	i = 0;
// 	count = 0;
// 	while (s[i])
// 	{
// 		while (s[i] && s[i] == c)
// 			i++;
// 		if (s[i])
// 			count++;
// 		while (s[i] && s[i] != c)
// 			i++;
// 	}
// 	return (count);
// }

// static char	*get_word(char const *s, char c, size_t index)
// {
// 	char	*res;
// 	size_t	len;
// 	size_t	i;

// 	len = 0;
// 	while (s[index + len] && s[index + len] != c)
// 		len++;
// 	res = malloc(sizeof(char) * (len + 1));
// 	if (!res)
// 		return (NULL);
// 	i = 0;
// 	while (i < len)
// 	{
// 		res[i] = s[index + i];
// 		i++;
// 	}
// 	res[i] = '\0';
// 	return (res);
// }

// char	**ft_split(char const *s, char c)
// {
// 	char	**res;
// 	size_t	i;
// 	size_t	j;

// 	res = malloc(sizeof(char *) * (count_word(s, c) + 1));
// 	if (!res)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while (i < count_word(s, c))
// 	{
// 		while (s[j] && s[j] == c)
// 			j++;
// 		if (s[j])
// 		{
// 			res[i] = get_word(s, c, j);
// 			if (!(res[i]))
// 				return (free_split(res, i));
// 			i++;
// 		}
// 		while (s[j] && s[j] != c)
// 			j++;
// 	}
// 	res[i] = NULL;
// 	return (res);
// }

t_word_list	*word_split(t_word *w)
{
	t_word_list	*result;

	if (w)
		result = list_string(w->word, (w->flags & F_QUOTED));
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
