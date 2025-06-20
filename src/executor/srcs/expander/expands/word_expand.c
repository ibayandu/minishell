/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:15:59 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 18:59:03 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "makers.h"
#include "libft.h"
#include "expander.h"

t_word_list	*expand_word_internal(t_word *word, int quoted, int *expanded_something)
{
	t_word_list	*list;
	t_word		*tword;
	char		*temp;
	char		*string;
	char		*istring;
	int			istring_size;
	int			istring_index;
	int			sindex;
	int			t_index;
	int			quoted_state;

	sindex = 0;
	string = word->word;
	istring = ft_malloc(1);
	istring_size = 1;
	istring_index = 0;
	quoted_state = UNQUOTED;
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
			temp = param_expand(string, &sindex, quoted, expanded_something);
			if (temp)
				return (NULL);
			istring = ft_strjoin(istring, temp);
			temp = NULL;
			continue ;
		}
		else if (string[sindex] == '"' && !quoted)
		{
			t_index = ++sindex;
			temp = string_extract_double_quoted(string, &sindex);
			quoted_state = (t_index == 1 && string[sindex] == '\0') ? WHOLLY_QUOTED : PARTIALLY_QUOTED;
			if (temp && *temp)
			{
				tword = alloc_word_desc();
				tword->word = temp;
				temp = NULL;
				list = expand_word_internal(tword, 1, NULL);
				if (!list)
					return (NULL);
				//dequote_list(list);
			}
			else
				list = NULL;
			if (list)
			{
				if (list->next)
					temp = string_list_internal(quote_list(list));
				else
					temp = ft_strdup(list->word->word);
			}
			else
				temp = NULL;
			if (temp)
			{
				temp = quote_string(temp);
				istring = ft_strjoin(istring, temp);
				continue ;
			}
		}
		istring = ft_realloc(istring, istring_size + 1);
		istring[istring_index++] = string[sindex];
		istring[istring_index] = '\0';
		sindex++;
	}
}
