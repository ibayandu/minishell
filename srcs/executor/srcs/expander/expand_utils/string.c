/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:06:26 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/04 13:04:57 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

static int	get_list_len(t_word_list *list)
{
	t_word_list	*t;
	int			res;

	res = 0;
	t = list;
	while (t)
	{
		if (t != list)
			res += 1;
		res += ft_strlen(t->word->word);
		t = t->next;
	}
	return (res);
}

char	*string_list(t_word_list *list)
{
	t_word_list	*t;
	char		*result;
	char		*r;
	size_t		result_size;

	if (!list)
		return (NULL);
	if (!list->next)
		return (ft_strdup(list->word->word));
	result_size = get_list_len(list);
	result = mem_malloc(result_size + 1);
	r = result;
	t = list;
	while (t)
	{
		if (t != list)
			*r++ = ' ';
		ft_strcpy(r, t->word->word);
		r += ft_strlen(t->word->word);
		t = t->next;
	}
	*r = '\0';
	return (result);
}

char	*string_extract_double_quoted(char *string, int *sindex)
{
	size_t	slen;
	int		j;
	int		i;
	char	*temp;
	int		skip;

	slen = ft_strlen(string + *sindex) + *sindex;
	temp = mem_malloc(1 + slen - *sindex);
	j = 0;
	i = *sindex - 1;
	skip = 0;
	while (string[++i])
	{
		if (string[i] != '"' && skip == 1)
			temp[j++] = string[i];
		else
		{
			if (skip == 2)
				break ;
			skip++;
		}
	}
	temp[j] = '\0';
	*sindex = i;
	return (temp);
}
