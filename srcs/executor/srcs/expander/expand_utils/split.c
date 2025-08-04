/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:04:08 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/04 13:05:22 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_makers.h"
#include "parser_utils.h"
#include "expander.h"

static char	*string_extract_verbatim(char *string, size_t slen, int *sindex,
		char *charlist)
{
	int		i;
	char	*temp;

	if (charlist[0] == '\'' && charlist[1] == '\0')
	{
		temp = string_extract_single_quoted(string, sindex);
		--*sindex;
		return (temp);
	}
	if (!*charlist)
	{
		temp = string + *sindex;
		*sindex += slen * !*sindex + ft_strlen(temp) * (*sindex);
		temp = ft_strdup(temp);
		return (temp);
	}
	i = *sindex - 1;
	while (string[++i])
		if (ft_strchr(charlist, string[i]))
			break ;
	temp = ft_substr(string, *sindex, i - *sindex);
	*sindex = i;
	return (temp);
}

static t_word	*get_next_word(char *string, int *sindex, char *seperators)
{
	t_word	*t;
	char	*current_word;

	while (string[*sindex] && ft_strchr(seperators, string[*sindex]))
		*sindex += 1;
	if (string[*sindex])
	{
		current_word = string_extract_verbatim(string, ft_strlen(string),
				sindex, seperators);
		if (!current_word)
			return (NULL);
		else
		{
			t = alloc_word_desc();
			t->word = current_word;
			if (!*current_word)
				t->word = ft_strdup("");
			return (t);
		}
	}
	return (NULL);
}

t_word_list	*list_string(char *string, char *seperators)
{
	t_word_list	*result;
	t_word		*t;
	int			sindex;

	sindex = 0;
	result = NULL;
	if (!string)
		return (NULL);
	while (string[sindex])
	{
		t = get_next_word(string, &sindex, seperators);
		if (!t)
			break ;
		result = make_word_list(t, result);
	}
	return (ft_revword(result));
}

static t_word_list	*word_split(t_word *w)
{
	t_word_list	*result;

	if (w)
		result = list_string(w->word, " \t\n");
	else
		result = NULL;
	return (result);
}

t_word_list	*word_list_split(t_word_list *list)
{
	t_word_list	*result;
	t_word_list	*tresult;
	t_word_list	*e;

	result = NULL;
	while (list)
	{
		tresult = word_split(list->word);
		if (!tresult && list->word)
			tresult = make_word_list(alloc_word_desc(), NULL);
		if (!result)
		{
			e = tresult;
			result = tresult;
		}
		else
		{
			e->next = tresult;
			while (e->next)
				e = e->next;
		}
		list = list->next;
	}
	return (result);
}
