/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:48:44 by yzeybek           #+#    #+#             */
/*   Updated: 2025/04/08 13:24:49 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLECTOR_H
# define COLLECTOR_H

# include <stdlib.h>

typedef struct s_memblock
{
	void				*data;
	struct s_memblock	*next;

}	t_memblock;

void	*ft_malloc(t_memblock **head, size_t size);
void	*ft_calloc(t_memblock **head, size_t type, size_t size);
void	*ft_realloc(t_memblock **head, void *old_data, size_t old_size,
			size_t new_size);
void	ft_free(t_memblock **head);

#endif // COLLECTOR_H
