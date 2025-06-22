/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_param.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 21:38:05 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/22 21:31:13 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "expander.h"
#include "minishell.h"
#include "makers.h"

unsigned int	hash_string(const char *s)
{
	unsigned int i;

	i = FNV_OFFSET;
	while (*s)
	{
		i += (i<<1) + (i<<4) + (i<<7) + (i<<8) + (i<<24);
		i ^= *s;
		s++;
	}
	return (i);
}

t_bucket	*hash_search(const char *string, t_hash *table)
{
	t_bucket		*list;
	int				bucket;
	unsigned int	hv;

	if (!table || !table->nentries)
		return (NULL);
	hv = hash_string(string);
	bucket = (hv & (table->nbuckets - 1));
	list = table->bucket_array ? table->bucket_array[bucket] : 0;
	while (list)
	{
		if (hv == list->khash && ft_strncmp(list->key, string, ft_strlen(string)) == 0)
		{
			list->times_found++;
			return (list);
		}
		list = list->next;
	}
	return (NULL);
}

t_variable	*find_variable_internal(const char *name, t_minishell *minishell)
{
	t_bucket	*res;
	t_variable	*var;
	t_context	*vc;

	var = NULL;
	vc = minishell->global_variables;
	while (vc)
	{
		res = hash_search(name, vc->table);
		if (res)
		{
			var = res->data;
			break ;
		}
		vc = vc->down;
	}
	if (!var)
		return (NULL);
	return (var);
}

t_variable	*new_shell_variable(const char *name)
{
	t_variable	*entry;

	entry = ft_malloc(sizeof(t_variable));
	entry->name = ft_strdup(name);
	entry->value = NULL;
	entry->exportstr = NULL;
	entry->attributes = 0;
	entry->context = 0;
	return (entry);
}

t_context	*new_var_context(char *name, int flags)
{
  t_context	*vc;

  vc = ft_malloc(sizeof(t_context));
  vc->name = name ? ft_strdup(name) : (char *)NULL;
  vc->flags = flags;
  vc->up = vc->down = NULL;
  vc->table = NULL;
  return vc;
}

t_hash	*hash_create(int buckets)
{
	t_hash	*new_table;
	int		i;

	new_table = ft_malloc(sizeof(t_hash));
	if (!buckets)
		buckets = DEFAULT_HASH_BUCKETS;
	new_table->bucket_array = ft_malloc(buckets * sizeof(t_bucket *));
	new_table->nbuckets = buckets;
	new_table->nentries = 0;
	i = -1;
	while (++i < buckets)
		new_table->bucket_array[i] = NULL;
	return (new_table);
}

void	create_variable_tables(t_minishell *minishell)
{
	if (!minishell->global_variables)
	{
		minishell->global_variables = new_var_context(NULL, 0);
		minishell->global_variables->scope = 0;
		minishell->global_variables->table = hash_create(VARIABLES_HASH_BUCKETS);
	}
}

int	legal_identifier(char *name)
{
	if (!name)
		return (0);
	while (ft_isalnum(*name) || *name == '_')
		name++;
	return (!*name);
}

void	hash_rehash(t_hash *table, int nsize)
{
	int			osize;
	int			i;
	int			j;
	t_bucket	**old_bucket_array;
	t_bucket	*item;
	t_bucket	*next;

	if (!table || nsize == table->nbuckets)
		return ;
	osize = table->nbuckets;
	old_bucket_array = table->bucket_array;
	table->nbuckets = nsize;
	table->bucket_array = ft_malloc(table->nbuckets * sizeof(t_bucket *));
	i = -1;
	while (++i < table->nbuckets)
		table->bucket_array[i] = NULL;
	j = -1;
	while (++j < osize)
	{
		item = old_bucket_array[j];
		while (item)
		{
			next = item->next;
			i = item->khash & (table->nbuckets - 1);
			item->next = table->bucket_array[i];
			table->bucket_array[i] = item;
			item = next;
		}
	}
}

void	hash_grow(t_hash *table)
{
  int nsize;

	nsize = table->nbuckets * HASH_REHASH_MULTIPLIER;
	if (nsize > 0)
		hash_rehash(table, nsize);
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
	item = ft_malloc(sizeof(t_bucket));
	item->next = table->bucket_array[bucket];
	table->bucket_array[bucket] = item;
	item->data = NULL;
	item->key = string;
	item->khash = hv;
	item->times_found = 0;
	table->nentries++;
	return (item);
}

t_variable	*make_new_variable(const char *name, t_hash *table, t_minishell *minishell)
{
	t_variable	*entry;
	t_bucket	*elt;

	entry = new_shell_variable(name);
	if (!minishell->global_variables)
		create_variable_tables(minishell);
	elt = hash_insert(ft_strdup(name), table);
	elt->data = (void *)entry;
	return (entry);
}

char	*make_variable_value(char *value)
{
	char	*retval;

	if (value)
	{
		if (*value)
			retval = ft_strdup(value);
		else
		{
			retval = ft_malloc(1);
			retval[0] = '\0';
		}
	}
	else
		retval = NULL;
	return retval;
}

t_variable	*bind_variable_internal(const char *name, char *value, t_hash *table, t_minishell *minishell)
{
	t_variable	*entry;
	t_bucket	*bucket;

	entry = NULL;
	bucket = hash_search(name, table);
	if (bucket)
		entry = bucket->data;
	if (entry)
	{
		entry = find_variable_internal(entry->name, minishell);
		if (!entry)
			return (entry);
	}
	else
	{
		entry = make_new_variable(name, table, minishell);
		entry->value = make_variable_value(value);
	}
	return (entry);
}

t_variable	*bind_variable(const char *name, char *value, t_minishell *minishell)
{
	t_variable	*v;
	t_context	*vc;
	t_context	*nvc;
	t_bucket	*b;

	if (!minishell->global_variables)
		create_variable_tables(minishell);
	vc = minishell->global_variables;
	while (vc)
	{
		v = NULL;
		b = hash_search(name, vc->table);
		if (b)
			v = b->data;
		nvc = vc;
		if (v)
			return (bind_variable_internal(v->name, value, nvc->table, minishell));
		vc = vc->down;
	}
	return (bind_variable_internal(name, value, minishell->global_variables->table, minishell));
}

t_bucket	*hash_remove (const char *string, t_hash *table)
{
	int				bucket;
	t_bucket		*prev;
	t_bucket		*temp;
	unsigned int	hv;

	if (!table || (table ? table->nentries : 0) == 0)
		return (NULL);
	hv = hash_string(string);
	bucket = (hv & (table->nbuckets - 1));
	prev = NULL;
	for (temp = table->bucket_array[bucket]; temp; temp = temp->next)
	{
		if (hv == temp->khash && ft_strncmp(temp->key, string, ft_strlen(string)) == 0)
		{
			if (prev)
				prev->next = temp->next;
			else
				table->bucket_array[bucket] = temp->next;
			table->nentries--;
			return (temp);
		}
		prev = temp;
	}
	return (NULL);
}

int	makunbound(const char *name, t_context *vc)
{
	t_bucket	*elt;
	t_context	*v;

	elt = NULL;
	v = vc;
	while (v)
	{
		elt = hash_remove(name, v->table);
		if (elt)
			break ;
		v = v->down;
	}
	if (!elt)
		return (-1);
	return (0);
}

int	unbind_variable(const char *name, t_minishell *minishell)
{
	t_variable	*v;
	int r;

	r = 1;
	v = find_variable_internal(name, minishell);
	if (v)
		r = makunbound(name, minishell->global_variables);
	return (r);
}

t_variable	*set_if_not(char *name, char *value, t_minishell *minishell)
{
	t_variable	*v;

	if (!minishell->global_variables)
		create_variable_tables(minishell);
	v = find_variable_internal(name, minishell);
	if (!v)
		v = bind_variable_internal(name, value, minishell->global_variables->table, minishell);
	return (v);
}

t_word	*param_expand(char *string, int *sindex, int *expanded_something, t_minishell *minishell)
{
	char		*temp;
	char		*temp1;
	int			zindex;
	int			t_index;
	char		c;
	t_variable	*var;
	t_word		*ret;

	zindex = *sindex;
	c = string[++zindex];
	temp = NULL;
	ret = NULL;
	if (c == '?')
		 temp = ft_itoa(minishell->last_command_exit_value);
	else
	{
		temp = NULL;
		t_index = zindex;
		while ((c = string[zindex]) && (ft_isalnum(c) || c == '_'))
			zindex++;
		temp1 = (zindex > t_index) ? ft_substr(string, t_index, zindex - t_index) : NULL;
		if (!temp1 || !*temp1)
		{
			temp = ft_strdup("$");
			if (expanded_something)
				*expanded_something = 0;
			goto return0;
		}
		var = find_variable_internal(temp1, minishell);
		if (var && var->value)
		{
			temp = var->value;
			goto return0;
		}
		temp = NULL;
		goto return0;
	}
	if (string[zindex])
		zindex++;
	return0:
	*sindex = zindex;
	if (ret == 0)
	{
		ret = alloc_word_desc();
		ret->flags = 0;
		ret->word = temp;
	}
	return ret;
}
