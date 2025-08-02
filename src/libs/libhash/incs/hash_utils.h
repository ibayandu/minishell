/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 16:15:35 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 16:39:47 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_UTILS_H
# define HASH_UTILS_H

# include "libhash.h"

# define HASH_REHASH_MULTIPLIER 4
# define FNV_OFFSET 2166136261

unsigned int	hash_string(const char *s);
t_bucket		*hash_remove(const char *string, t_hash *table);
void			hash_rehash(t_hash *table, int nsize);
void			hash_grow(t_hash *table);

#endif // HASH_UTILS_H
