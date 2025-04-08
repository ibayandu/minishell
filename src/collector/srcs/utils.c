/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:19:37 by yzeybek           #+#    #+#             */
/*   Updated: 2025/04/08 13:31:43 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/collector.h"

void	ft_add_block(t_memblock **head, t_memblock *new)
{
	t_memblock	*node;

	node = *head;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
}

t_memblock	*ft_new_block(void *data)
{
	t_memblock	*new;

	new = malloc(sizeof(t_memblock));
	if (!new)
		return (NULL);
	new->data = data;
	new->next = NULL;
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
		free(node->data);
		free(node);
		node = temp;
	}
	*head = NULL;
}
