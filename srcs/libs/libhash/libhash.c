/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libhash.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:54:32 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 19:19:17 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libmem.h"
#include "hash_utils.h"

t_bucket	*hash_search(const char *string, t_hash *table)
{
	t_bucket		*list;
	int				bucket;
	unsigned int	hv;

	if (!table || !table->nentries)
		return (NULL);
	hv = hash_string(string);
	bucket = (hv & (table->nbuckets - 1));
	list = NULL;
	if (table->bucket_array)
		list = table->bucket_array[bucket];
	while (list)
	{
		if (hv == list->khash
			&& ft_strncmp(list->key, string, ft_strlen(string)) == 0)
			return (list);
		list = list->next;
	}
	return (NULL);
}

t_hash	*hash_create(int buckets)
{
	t_hash	*new_table;
	int		i;

	new_table = mem_malloc(sizeof(t_hash));
	if (!buckets)
		buckets = DEFAULT_HASH_BUCKETS;
	new_table->bucket_array = mem_malloc(buckets * sizeof(t_bucket *));
	new_table->nbuckets = buckets;
	new_table->nentries = 0;
	i = -1;
	while (++i < buckets)
		new_table->bucket_array[i] = NULL;
	return (new_table);
}

t_bucket	*hash_insert(char *string, t_hash *table)
{
	t_bucket		*item;
	int				bucket;
	unsigned int	hv;

	if (!table)
		table = hash_create(0);
	item = NULL;
	if (table->nentries >= table->nbuckets * 2)
		hash_grow(table);
	hv = hash_string(string);
	bucket = (hv & (table->nbuckets - 1));
	item = mem_malloc(sizeof(t_bucket));
	item->next = table->bucket_array[bucket];
	table->bucket_array[bucket] = item;
	item->data = NULL;
	item->key = string;
	item->khash = hv;
	table->nentries++;
	return (item);
}

int	hash_makunbound(const char *name, t_hash *ht)
{
	t_bucket	*elt;
	t_hash		*h;

	elt = NULL;
	h = ht;
	if (h && h->nentries)
		elt = hash_remove(name, h);
	if (!elt)
		return (-1);
	return (0);
}
