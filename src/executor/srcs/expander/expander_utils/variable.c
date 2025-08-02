/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 20:11:17 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 21:06:07 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libmem.h"
#include "expander.h"

t_variable	*find_variable(const char *name, t_hash *ht)
{
	t_bucket	*res;
	t_variable	*var;

	var = NULL;
	res = NULL;
	if (ht)
		res = hash_search(name, ht);
	if (res)
		var = res->data;
	if (!var)
		return (NULL);
	return (var);
}

t_hash	*create_variable_tables(int table_index)
{
	static t_hash	*variable_tables[2];
	int				i;

	i = -1;
	while (!variable_tables[++i])
		variable_tables[i] = hash_create(VARIABLES_HASH_BUCKETS);
	return (variable_tables[table_index]);
}

t_variable	*make_new_variable(const char *name, t_hash *table)
{
	t_variable	*entry;
	t_bucket	*elt;

	entry = mem_malloc(sizeof(t_variable));
	entry->name = ft_strdup(name);
	entry->value = NULL;
	elt = hash_insert(ft_strdup(name), table);
	elt->data = (void *)entry;
	return (entry);
}

t_variable	*bind_variable(const char *name, char *value, t_hash *ht)
{
	t_variable	*entry;
	t_bucket	*bucket;

	entry = NULL;
	if (ht)
	{
		bucket = hash_search(name, ht);
		if (bucket)
			entry = bucket->data;
		if (entry)
			return (entry);
		entry = make_new_variable(name, ht);
		if (value)
			entry->value = ft_strdup(value);
	}
	return (entry);
}

int	unbind_variable(const char *name, t_hash *ht)
{
	t_variable	*v;
	int r;

	r = 1;
	if (ht)
	{
		v = find_variable(name, ht);
		if (v)
			r = hash_makunbound(name, ht);
	}
	return (r);
}
