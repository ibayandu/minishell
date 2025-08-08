/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:00:01 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/08 15:30:07 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

t_word_list	*list_append(t_word_list *head, t_word_list *tail)
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

char	**ft_realloc_vec(char **old, size_t new_items)
{
	size_t	old_count;
	char	**newv;
	size_t	to_copy;
	size_t	i;

	old_count = 0;
	if (old)
		while (old[old_count])
			old_count++;
	newv = mem_malloc((new_items + 1) * sizeof(char *));
	to_copy = new_items;
	if (old_count < new_items)
		to_copy = old_count;
	i = -1;
	while (++i < to_copy)
		newv[i] = old[i];
	i = to_copy - 1;
	while (++i < new_items)
		newv[i] = NULL;
	newv[new_items] = NULL;
	return (newv);
}
