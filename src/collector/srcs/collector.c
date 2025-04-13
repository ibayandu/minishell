/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:57:12 by yzeybek           #+#    #+#             */
/*   Updated: 2025/04/11 17:26:11 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/utils.h"
#include "incs/collector.h"

void	*ft_malloc(t_memblock **head, size_t size)
{
	void	*res;

	res = malloc(size);
	if (!res)
		return (NULL);
	ft_add_block(head, ft_new_block(res, size));
	return (res);
}

void	*ft_calloc(t_memblock **head, size_t count, size_t size)
{
	unsigned char	*ptr;
	void			*res;
	size_t			i;

	res = malloc(size * count);
	if (!res)
		return (NULL);
	ptr = res;
	i = -1;
	while (++i < size * count)
		ptr[i] = '\0';
	ft_add_block(head, ft_new_block(res, size * count));
	return (res);
}

void	*ft_realloc(t_memblock **head, void *old_data, size_t new_size)
{
	size_t					i;
	void					*res;
	unsigned char			*new_ptr;
	unsigned char *const	old_ptr = old_data;
	size_t					old_size;

	res = malloc(new_size);
	if (!res)
		return (NULL);
	i = -1;
	new_ptr = res;
	old_size = ft_find_size(head, old_data);
	while (++i < (old_size * (old_size < new_size)) + (new_size
			* (new_size < old_size)))
		new_ptr[i] = old_ptr[i];
	ft_add_block(head, ft_new_block(res, new_size));
	return (res);
}

void	ft_free(t_memblock **head)
{
	ft_clear_block(head);
}
