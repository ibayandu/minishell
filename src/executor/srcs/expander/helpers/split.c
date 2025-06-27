/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:04:08 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/22 21:35:30 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "makers.h"
#include "utils.h"
#include "token.h"
#include "expander.h"

char	*string_extract_verbatim(char *string, size_t slen, int *sindex, char *charlist)
{
	int		i;
	int		c;
	char	*temp;

	if (charlist[0] == '\'' && charlist[1] == '\0')
	{
		temp = string_extract_single_quoted(string, sindex);
		--*sindex;
		return temp;
	}
	if (!*charlist)
	{
		temp = string + *sindex;
		c = (*sindex == 0) ? slen : ft_strlen(temp);
		temp = ft_strdup(temp);
		*sindex += c;
		return (temp);
	}
	i = *sindex;
	while ((c = string[i]))
	{
		if (ft_strchr(charlist, c))
			break;
		i++;
	}
	temp = ft_substr(string, *sindex, i - *sindex);
	*sindex = i;
	return (temp);
}

t_word_list	*list_string(char *string, char *seperators)
{
	t_word_list	*result;
	t_word		*t;
	int			sindex;
	char		*current_word;

	sindex = 0;
	result = NULL;
	if (!string)
		return (NULL);
	while (string[sindex])
	{
		while (string[sindex] && ft_strchr(seperators, string[sindex]))
			sindex++;
		if (string[sindex])
		{
			current_word = string_extract_verbatim(string, ft_strlen(string), &sindex, seperators);
			if (!current_word)
				break;
			else if (!*current_word)
			{
				t = alloc_word_desc();
				t->word = ft_strdup("");
				result = make_word_list(t, result);
			}
			else
			{
				t = alloc_word_desc();
				t->word = current_word;
				result = make_word_list(t, result);
			}
		}
	}
	return (ft_revword(result));
}

t_word_list	*word_split(t_word *w)
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
