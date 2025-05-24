/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:48:44 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/24 22:53:40 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLECTOR_H
# define COLLECTOR_H

# include <stddef.h>

typedef struct s_memblock
{
	void				*data;
	size_t				size;
	struct s_memblock	**head;
	struct s_memblock	*next;

}						t_memblock;

void					*ft_malloc(size_t size);
void					*ft_calloc(size_t type, size_t size);
void					*ft_realloc(void *old_data, size_t new_size);
void					ft_free(void);
void					*ft_absorb(void *addr);

#endif // COLLECTOR_H
