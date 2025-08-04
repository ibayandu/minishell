/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libhash.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:54:34 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/04 13:00:43 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBHASH_H
# define LIBHASH_H

# define DEFAULT_HASH_BUCKETS 128

typedef struct s_bucket
{
	struct s_bucket	*next;
	char			*key;
	void			*data;
	unsigned int	khash;

}	t_bucket;

typedef struct s_hash
{
	t_bucket	**bucket_array;
	int			nbuckets;
	int			nentries;

}	t_hash;

t_bucket	*hash_search(const char *string, t_hash *table);
t_hash		*hash_create(int buckets);
t_bucket	*hash_insert(char *string, t_hash *table);
int			hash_makunbound(const char *name, t_hash *ht);

#endif // LIBHASH_H
