/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 21:13:17 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/13 19:45:17 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/libft.h"

t_list	*ft_lstnew(t_memblock **head, void *content)
{
	t_list	*new_node;

	new_node = (t_list *)ft_malloc(head, sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}
