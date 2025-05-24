/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_listrev.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:24:49 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/20 18:47:37 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "structs.h"

t_word_list	*ft_revword(t_word_list *list)
{
	t_word_list	*next;
	t_word_list	*prev;

	if (!list)
		return (list);
	prev = NULL;
	while (list)
	{
		next = list->next;
		list->next = prev;
		prev = list;
		list = next;
	}
	return (prev);
}

t_redirect	*ft_revredir(t_redirect *list)
{
	t_redirect	*next;
	t_redirect	*prev;

	if (!list)
		return (list);
	prev = NULL;
	while (list)
	{
		next = list->next;
		list->next = prev;
		prev = list;
		list = next;
	}
	return (prev);
}

