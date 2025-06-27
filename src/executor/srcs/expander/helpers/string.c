/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:06:26 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/23 21:39:19 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"

char	*string_list_internal(t_word_list *list)
{
	t_word_list	*t;
	char		*result;
	char		*r;
	size_t		word_len;
	size_t		sep_len;
	size_t		result_size;

	if (!list)
		return (NULL);
	if (!list->next)
		return (ft_strdup(list->word->word));
	sep_len = 1;
	result_size = 0;
	t = list;
	while (t)
	{
		if (t != list)
			result_size += sep_len;
		result_size += ft_strlen(t->word->word);
		t = t->next;
	}
	r = result = ft_malloc (result_size + 1);
	t = list;
	while (t)
	{
		if (t != list && sep_len)
		{
			if (sep_len > 1)
			{
				ft_strlcpy(r, " ", sep_len);
				r += sep_len;
			}
			else
				*r++ = ' ';
		}
		word_len = ft_strlen(t->word->word);
		ft_strcpy(r, t->word->word);
		r += word_len;
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

	slen = ft_strlen(string + *sindex) + *sindex;
	temp = ft_malloc(1 + slen - *sindex);
	j = 0;
	i = *sindex;
	while (string[i])
	{
		if (string[i] != '"')
		{
			temp[j++] = string[i++];
			continue;
		}
		else
		{
			i++;
			continue;
		}
		break;
	}
	temp[j] = '\0';
	if (string[i])
		i++;
	*sindex = i;
	return (temp);
}
