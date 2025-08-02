/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_listrev.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:24:49 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 18:26:22 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

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
