/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_param.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 21:38:05 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/22 06:17:11 by yzeybek          ###   ########.tr       */
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
		temp1 = (zindex > t_index) ? ft_substr(string, t_index, zindex) : NULL;
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
