/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:28:05 by yzeybek           #+#    #+#             */
/*   Updated: 2025/04/14 20:50:04 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "incs/collector.h"

t_memblock	*ft_add_new_block(void *data, size_t size);
void		ft_clear_block(t_memblock **head);
size_t		ft_find_size(void *data);

#endif // UTILS_H
