/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libmem.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:48:44 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 16:56:10 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMEM_H
# define LIBMEM_H

# include <stddef.h>

typedef struct s_mem_block
{
	void				*data;
	size_t				size;
	struct s_mem_block	**head;
	struct s_mem_block	*next;

}	t_mem_block;

void	*mem_malloc(size_t size);
void	*mem_calloc(size_t type, size_t size);
void	*mem_realloc(void *old_data, size_t new_size);
void	mem_free(void);
void	*mem_absorb(void *addr);

#endif // LIBMEM_H
