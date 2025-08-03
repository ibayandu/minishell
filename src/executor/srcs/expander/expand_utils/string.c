/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:06:26 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 21:06:29 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libmem.h"
#include "expander.h"

char	*string_list(t_word_list *list)
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
	r = result = mem_malloc(result_size + 1);
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
	int		skip;

	slen = ft_strlen(string + *sindex) + *sindex;
	temp = mem_malloc(1 + slen - *sindex);
	j = 0;
	i = *sindex;
	skip = 0;
	while (string[i])
	{
		if (string[i] != '"' && skip == 1)
		{
			temp[j++] = string[i++];
			continue ;
		}
		else
		{
			if (skip == 2)
				break ;
			i++;
			skip++;
			continue ;
		}
		break;
	}
	temp[j] = '\0';
	*sindex = i;
	return (temp);
}

int	legal_identifier(char *name)
{
	int	i;

	i = 0;
	if (!name)
		return (i);
	if (ft_isdigit(name[i]))
		return (i);
	while (ft_isalnum(name[i]) || name[i] == '_')
		i++;
	return (!name[i]);
}
