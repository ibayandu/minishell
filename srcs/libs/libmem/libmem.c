/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libmem.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:57:12 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 16:43:02 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libmem.h"
#include "mem_utils.h"

void	*mem_malloc(size_t size)
{
	void	*res;

	res = malloc(size);
	if (!res)
		return (mem_exit());
	mem_add_new_block(res, size);
	return (res);
}

void	*mem_calloc(size_t count, size_t size)
{
	unsigned char	*ptr;
	void			*res;
	size_t			i;

	res = malloc(size * count);
	if (!res)
		return (mem_exit());
	ptr = res;
	i = -1;
	while (++i < size * count)
		ptr[i] = '\0';
	mem_add_new_block(res, size * count);
	return (res);
}

void	*mem_realloc(void *old_data, size_t new_size)
{
	size_t					i;
	void					*res;
	unsigned char			*new_ptr;
	size_t					old_size;
	unsigned char *const	old_ptr = old_data;

	res = malloc(new_size);
	if (!res)
		return (mem_exit());
	i = -1;
	new_ptr = res;
	old_size = mem_find_size(old_data);
	while (++i < (old_size * (old_size < new_size)) + (new_size
			* (new_size < old_size)))
		new_ptr[i] = old_ptr[i];
	mem_add_new_block(res, new_size);
	return (res);
}

void	mem_free(void)
{
	mem_clear_block(mem_add_new_block(NULL, 0)->head);
}

void	*mem_absorb(void *addr)
{
	mem_add_new_block(addr, 1);
	return (addr);
}
