/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:40:03 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/12 15:01:55 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

t_word_list	*list_append(t_word_list *head, t_word_list *tail)
{
	t_word_list	*t_head;

	if (!head)
		return (tail);
	t_head = head;
	while (t_head->next)
		t_head = t_head->next;
	t_head->next = tail;
	return (head);
}

static char	*mk_env_string(const char *name, const char *value)
{
	size_t	name_len;
	size_t	value_len;
	char	*p;
	char	*q;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	p = mem_malloc(2 + name_len + value_len);
	ft_memcpy(p, name, name_len);
	q = p + name_len;
	if (value)
		q[0] = '=';
	if (value && *value)
		ft_memcpy(q + 1, value, value_len + 1);
	else
		q[1 - (!value)] = '\0';
	return (p);
}

static char	**make_env_array_from_var_list(t_variable **vars, int is_export)
{
	int			i;
	int			list_index;
	t_variable	*var;
	char		**list;
	char		*value;

	list = mem_malloc((1 + strvec_len((char **)vars)) * sizeof(char *));
	i = 0;
	list_index = 0;
	var = vars[i];
	while (var)
	{
		value = var->value;
		if (value || is_export)
		{
			list[list_index] = mk_env_string(var->name, value);
			list_index++;
		}
		i++;
		var = vars[i];
	}
	list[list_index] = NULL;
	return (list);
}

static int	export_environment_candidate(t_variable *var)
{
	if (var)
		return (1);
	return (0);
}

char	**make_var_export_array(t_hash *ht, int is_export)
{
	char		**list;
	t_variable	**vars;

	vars = map_over(export_environment_candidate, ht);
	if (!vars)
		return (NULL);
	list = make_env_array_from_var_list(vars, is_export);
	return (list);
}
