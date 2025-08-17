/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:21:53 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/15 01:21:29 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

static t_word_list	*glob_append(char *word)
{
	char		**glob_array;
	int			glob_index;
	t_word_list	*res;

	glob_array = strvec_sort(glob_filename(word, 0), 1);
	if (!glob_array)
		glob_array = mem_calloc(sizeof(char *), 1);
	res = NULL;
	glob_index = -1;
	while (glob_array[++glob_index])
		res = make_word_list(make_bare_word(glob_array[glob_index]), res);
	return (res);
}

t_word_list	*glob_list(t_word_list *tlist)
{
	t_word_list	*glob_list;
	t_word_list	*output_list;
	t_word_list	*next;

	output_list = NULL;
	while (tlist)
	{
		next = tlist->next;
		if (glob_pattern(tlist->word->word))
			glob_list = glob_append(tlist->word->word);
		if (glob_pattern(tlist->word->word) && glob_list)
			output_list = list_append(glob_list, output_list);
		else
		{
			tlist->next = output_list;
			output_list = tlist;
		}
		tlist = next;
	}
	return (glob_restar(output_list));
}
