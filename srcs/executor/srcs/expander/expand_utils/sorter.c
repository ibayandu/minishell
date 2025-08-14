/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 09:50:42 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/14 01:48:52 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

static size_t	count_strings(char **arr)
{
	size_t	count;

	count = 0;
	while (arr && arr[count])
		count++;
	return (count);
}

static void	swap_strings(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	bubble_sort_strings(char **arr, size_t count, int is_asc)
{
	size_t	i;
	size_t	j;
	int		cmp;

	i = 0;
	while (i + 1 < count)
	{
		j = 0;
		while (j + 1 < count - i)
		{
			cmp = ft_strcmp(arr[j], arr[j + 1]);
			if ((is_asc && cmp > 0) || (!is_asc && cmp < 0))
				swap_strings(&arr[j], &arr[j + 1]);
			j++;
		}
		i++;
	}
}

char	**strvec_sort(char **input, int is_asc)
{
	size_t	count;
	size_t	i;
	char	**sorted;

	count = count_strings(input);
	if (count == 0)
		return (NULL);
	sorted = mem_malloc((count + 1) * sizeof(char *));
	if (!sorted)
		return (NULL);
	i = 0;
	while (i < count)
	{
		sorted[i] = ft_strdup(input[i]);
		i++;
	}
	sorted[count] = NULL;
	bubble_sort_strings(sorted, count, is_asc);
	return (sorted);
}
