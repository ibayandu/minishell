/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 16:16:05 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 19:18:49 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "libft.h"
#include "hash_utils.h"

unsigned int	hash_string(const char *s)
{
	unsigned int	i;
	int				j;

	i = FNV_OFFSET;
	j = 0;
	while (s[j])
	{
		i += (i << 1) + (i << 4) + (i << 7) + (i << 8) + (i << 24);
		i ^= s[j];
		j++;
	}
	return (i);
}

t_bucket	*hash_remove(const char *string, t_hash *table)
{
	int				bucket;
	t_bucket		*prev;
	t_bucket		*temp;
	unsigned int	hv;

	hv = hash_string(string);
	bucket = (hv & (table->nbuckets - 1));
	prev = NULL;
	temp = table->bucket_array[bucket];
	while (temp)
	{
		if (hv == temp->khash
			&& ft_strncmp(temp->key, string, ft_strlen(string)) == 0)
		{
			if (prev)
				prev->next = temp->next;
			else
				table->bucket_array[bucket] = temp->next;
			table->nentries--;
			return (temp);
		}
		prev = temp;
		temp = temp->next;
	}
	return (NULL);
}

void	hash_rehash(t_hash *table, int nsize)
{
	int			i;
	int			j;
	t_bucket	**old_bucket_array;
	t_bucket	*item;
	t_bucket	*next;

	old_bucket_array = table->bucket_array;
	table->bucket_array = mem_calloc(nsize, sizeof(t_bucket *));
	j = -1;
	while (++j < table->nbuckets)
	{
		item = old_bucket_array[j];
		while (item)
		{
			next = item->next;
			i = item->khash & (nsize - 1);
			item->next = table->bucket_array[i];
			table->bucket_array[i] = item;
			item = next;
		}
	}
	table->nbuckets = nsize;
}

void	hash_grow(t_hash *table)
{
	int	nsize;

	nsize = table->nbuckets * HASH_REHASH_MULTIPLIER;
	if (nsize > 0)
		hash_rehash(table, nsize);
}
