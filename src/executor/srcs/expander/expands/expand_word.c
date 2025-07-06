/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:15:59 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/06 21:35:50 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "makers.h"
#include "libft.h"
#include "utils.h"
#include "expander.h"

char *replace_star_unquoted(const char *input)
{
	size_t len = ft_strlen(input);
	char *out  = ft_malloc(len + 1);
	if (!out) return NULL;

	int in_sq = 0, in_dq = 0;
	size_t j = 0;

	for (size_t i = 0; i < len; i++)
	{
		char c = input[i];
		if (c == '\'' && !in_dq)
		{
			// toggle single‑quote state, copy the quote itself
			in_sq = !in_sq;
			out[j++] = c;
		}
		else if (c == '"' && !in_sq)
		{
			// toggle double‑quote state, copy the quote
			in_dq = !in_dq;
			out[j++] = c;
		}
		else if (c == '*' && !in_sq && !in_dq)
		{
			// unquoted '*': replace
			out[j++] = '\001';
		}
		else
		{
			// any other character
			out[j++] = c;
		}
	}

	out[j] = '\0';
	return out;
}

t_word_list	*expand_word(t_word *word, int quoted, int *expanded_something, t_minishell *minishell)
{
	t_word_list	*list;
	t_word		*tword;
	char		*temp;
	char		*string;
	char		*istring;
	int			istring_size;
	int			istring_index;
	int			sindex;

	sindex = 0;
	string = word->word;
	istring = ft_strdup("");
	istring_size = 1;
	istring_index = 0;
	temp = NULL;
	while (1)
	{
		if (!string)
			break;
		if (!string[sindex])
			break;
		else if (string[sindex] == '$')
		{
			if (expanded_something)
				*expanded_something = 1;
			tword = param_expand(string, &sindex, expanded_something, minishell);
			if (!tword)
				return (NULL);
			temp = tword ? tword->word : NULL;
			temp = replace_star_unquoted(temp);
			istring = ft_strjoin(istring, temp);
			istring_size = ft_strlen(istring) + 1;
			istring_index += ft_strlen(temp);
			temp = NULL;
			continue ;
		}
		else if (string[sindex] == '*' && !quoted)
			string[sindex] = '\001';
		else if (string[sindex] == '"')
		{
			temp = string_extract_double_quoted(string, &sindex);
			if (temp && *temp)
			{
				if (!ft_strchr(temp, '*'))
					word->flags &= ~F_QUOTED;
				else
					word->flags |= F_QUOTED;
				tword = alloc_word_desc();
				tword->word = temp;
				temp = NULL;
				list = expand_word(tword, 1, NULL, minishell);
				if (!list)
					return (NULL);
			}
			else
				list = NULL;
			if (list)
			{
				if (list->next)
					temp = string_list(list);
				else
					temp = ft_strdup(list->word->word);
			}
			else
				temp = NULL;
			if (temp)
			{
				istring = ft_strjoin(istring, temp);
				istring_size = ft_strlen(istring) + 1;
				istring_index += ft_strlen(temp);
			}
			continue ;
		}
		else if (string[sindex] == '\'' && !quoted)
		{
			temp = ft_malloc(ft_strlen(&string[sindex]) + 1);
			temp = string_extract_single_quoted(string, &sindex);
			if (!*temp)
				temp = NULL;
			if (temp)
			{
				if (!ft_strchr(temp, '*'))
					word->flags &= ~F_QUOTED;
				else
					word->flags |= F_QUOTED;
				istring = ft_strjoin(istring, temp);
				istring_size = ft_strlen(istring) + 1;
				istring_index += ft_strlen(temp);
			}
			continue ;
		}
		istring = ft_realloc(istring, istring_size + 1);
		istring[istring_index++] = string[sindex];
		istring[istring_index] = '\0';
		istring_size++;
		sindex++;
	}
	tword = alloc_word_desc();
	tword->word = istring;
	istring = NULL;
	list = make_word_list(tword, NULL);
	tword->flags = word->flags;
	return (list);
}
