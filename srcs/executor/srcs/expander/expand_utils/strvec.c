/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:00:01 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/12 15:01:59 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

int	strvec_len(char **array)
{
	int	i;

	if (!array)
		return (0);
	i = 0;
	while (array[i])
		i++;
	return (i);
}

char	**strvec_shift(char **vec)
{
	size_t	i;
	int		n;

	n = 0;
	while (vec[n] && *vec[n])
		n++;
	i = n;
	vec[i - n] = vec[i];
	while (vec[i++])
		vec[i - n] = vec[i];
	return (vec);
}

char	**strvec_cpy(char **dest, char **src)
{
	int	i;

	i = -1;
	while (src[++i])
		dest[i] = src[i];
	dest[i] = NULL;
	return (dest);
}

char	**strvec_realloc(char **old, size_t new_items)
{
	size_t	old_count;
	char	**newv;
	size_t	to_copy;
	size_t	i;

	old_count = 0;
	if (old)
		while (old[old_count])
			old_count++;
	newv = mem_malloc((new_items + 1) * sizeof(char *));
	to_copy = new_items;
	if (old_count < new_items)
		to_copy = old_count;
	i = -1;
	while (++i < to_copy)
		newv[i] = old[i];
	i = to_copy - 1;
	while (++i < new_items)
		newv[i] = NULL;
	newv[new_items] = NULL;
	return (newv);
}
