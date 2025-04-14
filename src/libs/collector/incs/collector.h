/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:48:44 by yzeybek           #+#    #+#             */
/*   Updated: 2025/04/14 21:20:27 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLECTOR_H
# define COLLECTOR_H

# include <stdlib.h>

typedef struct s_memblock
{
	void				*data;
	size_t				size;
	struct s_memblock	**head;
	struct s_memblock	*next;

}	t_memblock;

void	*ft_malloc(size_t size);
void	*ft_calloc(size_t type, size_t size);
void	*ft_realloc(void *old_data, size_t new_size);
void	ft_free(void);

#endif // COLLECTOR_H
