/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:15:59 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/22 21:20:37 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "makers.h"
#include "libft.h"
#include "utils.h"
#include "expander.h"

t_word_list	*expand_word_internal(t_word *word, int quoted, int *expanded_something, t_minishell *minishell)
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
	istring = ft_malloc(1);
	istring_size = 1;
	istring_index = 0;
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
			istring = ft_strjoin(istring, temp);
			istring_size = ft_strlen(istring) + 1;
			istring_index += ft_strlen(temp);
			temp = NULL;
			continue ;
		}
		else if (string[sindex] == '"' && !quoted)
		{
			temp = string_extract_double_quoted(string, &sindex);
			if (temp && *temp)
			{
				tword = alloc_word_desc();
				tword->word = temp;
				temp = NULL;
				list = expand_word_internal(tword, 1, NULL, minishell);
				if (!list)
					return (NULL);
			}
			else
				list = NULL;
			if (list)
			{
				if (list->next)
					temp = string_list_internal(list);
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
