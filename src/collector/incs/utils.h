/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:28:05 by yzeybek           #+#    #+#             */
/*   Updated: 2025/04/08 13:31:31 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "incs/collector.h"

void		ft_add_block(t_memblock **head, t_memblock *new);
t_memblock	*ft_new_block(void *data);
void		ft_clear_block(t_memblock **head);

#endif // UTILS_H
