/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:19:37 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/16 20:04:45 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "collector.h"

t_memblock	*ft_add_new_block(void *data, size_t size)
{
	static t_memblock	*head;
	t_memblock			*new;
	t_memblock			*node;

	new = malloc(sizeof(t_memblock));
	if (!new)
		return (NULL);
	new->data = data;
	new->size = size;
	new->head = &head;
	new->next = NULL;
	if (!head)
		head = new;
	else
	{
		node = head;
		while (node->next)
			node = node->next;
		node->next = new;
	}
	return (new);
}

void	ft_clear_block(t_memblock **head)
{
	t_memblock	*temp;
	t_memblock	*node;

	if (!(*head))
		return ;
	node = *head;
	while (node)
	{
		temp = node->next;
		if (node->data)
			free(node->data);
		free(node);
		node = temp;
	}
	*head = NULL;
}

size_t	ft_find_size(void *data)
{
	t_memblock	*node;
	size_t		res;

	node = *(ft_add_new_block(NULL, 0)->head);
	while (1)
	{
		if (node->data == data)
			res = node->size;
		if (node->next)
			node = node->next;
		else
			break ;
	}
	return (res);
}
